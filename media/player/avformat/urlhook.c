/*
 * This file is part of AirStashPlayer.
 * Copyright (c) 2016 Wearable Inc.
 *
 * AirStashPlayer is based on ijkPlayer:
 * Copyright (c) 2013-2014 Zhang Rui <bbcallen@gmail.com>
 *
 * Portions of ijkPlayer are based on kxmovie:
 * Copyright (c) 2012 Konstantin Boukreev. All rights reserved.
 *
 * AirStashPlayer is free software: you can redistribute it and/or
 * modify it under the terms of version 3 of the GNU Lesser General
 * Public License as published by the Free Software Foundation.
 *
 * AirStashPlayer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with AirStashPlayer.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include <assert.h>
#include "libavformat/avformat.h"
#include "libavformat/url.h"
#include "libavutil/avstring.h"
#include "libavutil/log.h"
#include "libavutil/opt.h"

#include "player/avutil/opt.h"
#include "avformat.h"

typedef struct Context {
    AVClass        *class;
    URLContext     *inner;

    int64_t         logical_pos;
    int64_t         logical_size;
    int             io_error;

    AirStashAVInject_OnUrlOpenData inject_data;
    const char     *scheme;
    const char     *inner_scheme;
    int             open_callback_id;

    /* options */
    int             inner_flags;
    AVDictionary   *inner_options;
    int64_t         opaque;
    int             segment_index;
    int64_t         test_fail_point;
    int64_t         test_fail_point_next;
} Context;

static int airstashurlhook_call_inject(URLContext *h)
{
    Context *c = h->priv_data;
    AirStashAVInjectCallback inject_callback = airstashav_get_inject_callback();
    void *opaque = (void *) (intptr_t) c->opaque;
    int ret = 0;

    if (ff_check_interrupt(&h->interrupt_callback)) {
        ret = AVERROR_EXIT;
        goto fail;
    }

    if (opaque && inject_callback) {
        AirStashAVInject_OnUrlOpenData inject_data_backup = c->inject_data;

        c->inject_data.is_handled = 0;
        c->inject_data.is_url_changed = 0;
        ret = inject_callback(opaque, c->open_callback_id, &c->inject_data, sizeof(c->inject_data));
        if (ret || !c->inject_data.url[0]) {
            ret = AVERROR_EXIT;
            goto fail;
        }

        if (!c->inject_data.is_url_changed && strcmp(inject_data_backup.url, c->inject_data.url)) {
            // force a url compare
            c->inject_data.is_url_changed = 1;
        }

        av_log(h, AV_LOG_INFO, "%s %s (%s)\n", h->prot->name, c->inject_data.url, c->inject_data.is_url_changed ? "changed" : "remain");
    }

    if (ff_check_interrupt(&h->interrupt_callback)) {
        ret = AVERROR_EXIT;
        goto fail;
    }

fail:
    return ret;
}

static int airstashurlhook_reconnect(URLContext *h, AVDictionary *extra)
{
    Context *c = h->priv_data;
    int ret = 0;
    URLContext *new_url = NULL;
    AVDictionary *inner_options = NULL;

    c->test_fail_point_next += c->test_fail_point;

    assert(c->inner_options);
    av_dict_copy(&inner_options, c->inner_options, 0);
    if (extra)
        av_dict_copy(&inner_options, extra, 0);

    ret = ffurl_open(&new_url, c->inject_data.url, c->inner_flags, &h->interrupt_callback, &inner_options);
    if (ret)
        goto fail;

    ffurl_closep(&c->inner);

    c->inner        = new_url;
    h->is_streamed  = c->inner->is_streamed;
    c->logical_pos  = ffurl_seek(c->inner, 0, SEEK_CUR);
    if (c->inner->is_streamed)
        c->logical_size = -1;
    else
        c->logical_size = ffurl_seek(c->inner, 0, AVSEEK_SIZE);

    c->io_error = 0;
fail:
    av_dict_free(&inner_options);
    return ret;
}

static int airstashurlhook_init(URLContext *h, const char *arg, int flags, AVDictionary **options)
{
    Context *c = h->priv_data;
    int ret = 0;

    av_strstart(arg, c->scheme, &arg);

    c->inner_flags = flags;

    if (options)
        av_dict_copy(&c->inner_options, *options, 0);
    av_dict_set_int(&c->inner_options, "airstashinject-opaque",        c->opaque, 0);
    av_dict_set_int(&c->inner_options, "airstashinject-segment-index", c->segment_index, 0);

    c->inject_data.size = sizeof(c->inject_data);
    c->inject_data.segment_index = c->segment_index;
    c->inject_data.retry_counter = 0;

    if (av_strstart(arg, c->inner_scheme, NULL)) {
        snprintf(c->inject_data.url, sizeof(c->inject_data.url), "%s", arg);
    } else {
        snprintf(c->inject_data.url, sizeof(c->inject_data.url), "%s%s", c->inner_scheme, arg);
    }

    return ret;
}

static int airstashtcphook_open(URLContext *h, const char *arg, int flags, AVDictionary **options)
{
    Context *c = h->priv_data;
    int ret = 0;

    c->scheme = "airstashtcphook:";
    c->inner_scheme = "tcp:";
    c->open_callback_id = AIRSTASHAVINJECT_ON_TCP_OPEN;
    ret = airstashurlhook_init(h, arg, flags, options);
    if (ret)
        goto fail;

    ret = airstashurlhook_call_inject(h);
    if (ret)
        goto fail;

    ret = airstashurlhook_reconnect(h, NULL);
    if (ret)
        goto fail;

fail:
    return ret;
}

static int airstashurlhook_close(URLContext *h)
{
    Context *c = h->priv_data;

    av_dict_free(&c->inner_options);
    return ffurl_closep(&c->inner);
}

static int airstashurlhook_read(URLContext *h, unsigned char *buf, int size)
{
    Context *c = h->priv_data;
    int ret = 0;

    if (c->io_error < 0)
        return c->io_error;

    if (c->test_fail_point_next > 0 && c->logical_pos >= c->test_fail_point_next) {
        av_log(h, AV_LOG_ERROR, "test fail point:%"PRId64"\n", c->test_fail_point_next);
        c->io_error = AVERROR(EIO);
        return AVERROR(EIO);
    }

    ret = ffurl_read(c->inner, buf, size);
    if (ret > 0)
        c->logical_pos += ret;
    else
        c->io_error = ret;

    return ret;
}

static int airstashurlhook_write(URLContext *h, const unsigned char *buf, int size)
{
    Context *c = h->priv_data;

    return ffurl_write(c->inner, buf, size);
}

static int64_t airstashurlhook_seek(URLContext *h, int64_t pos, int whence)
{
    Context *c = h->priv_data;
    int64_t seek_ret = 0;

    seek_ret = ffurl_seek(c->inner, pos, whence);
    if (seek_ret < 0) {
        c->io_error = (int)seek_ret;
        return seek_ret;
    }

    c->logical_pos = seek_ret;
    if (c->test_fail_point)
        c->test_fail_point_next = c->logical_pos + c->test_fail_point;

    c->io_error = 0;
    return seek_ret;
}

static int airstashhttphook_reconnect_at(URLContext *h, int64_t offset)
{
    AVDictionary *extra_opts = NULL;

    av_dict_set_int(&extra_opts, "offset", offset, 0);
    int ret = airstashurlhook_reconnect(h, extra_opts);
    av_dict_free(&extra_opts);
    return ret;
}

static int airstashhttphook_open(URLContext *h, const char *arg, int flags, AVDictionary **options)
{
    Context *c = h->priv_data;
    int ret = 0;

    c->scheme = "airstashhttphook:";
    c->inner_scheme = "http:";
    c->open_callback_id = AIRSTASHAVINJECT_ON_HTTP_OPEN;

    ret = airstashurlhook_init(h, arg, flags, options);
    if (ret)
        goto fail;

    ret = airstashurlhook_call_inject(h);
    if (ret)
        goto fail;

    ret = airstashurlhook_reconnect(h, NULL);
    while (ret) {
        int inject_ret = 0;

        switch (ret) {
            case AVERROR_EXIT:
                goto fail;
        }

        c->inject_data.retry_counter++;
        inject_ret = airstashurlhook_call_inject(h);
        if (inject_ret) {
            ret = AVERROR_EXIT;
            goto fail;
        }

        if (!c->inject_data.is_handled)
            goto fail;

        av_log(h, AV_LOG_INFO, "%s: will reconnect at start\n", __func__);
        ret = airstashurlhook_reconnect(h, NULL);
        av_log(h, AV_LOG_INFO, "%s: did reconnect at start: %d\n", __func__, ret);
        if (ret)
            c->inject_data.retry_counter++;
    }

fail:
    return ret;
}

static int airstashhttphook_read(URLContext *h, unsigned char *buf, int size)
{
    Context *c = h->priv_data;
    int ret = 0;
    int read_ret = AVERROR(EIO);

    c->inject_data.retry_counter = 0;

    read_ret = airstashurlhook_read(h, buf, size);
    while (read_ret < 0 && !h->is_streamed && c->logical_pos < c->logical_size) {
        c->io_error = read_ret;
        switch (read_ret) {
            case AVERROR_EXIT:
                goto fail;
        }

        c->inject_data.retry_counter++;
        ret = airstashurlhook_call_inject(h);
        if (ret) {
            read_ret = AVERROR_EXIT;
            goto fail;
        }

        if (!c->inject_data.is_handled)
            goto fail;

        av_log(h, AV_LOG_INFO, "%s: will reconnect(%d) at %"PRId64"\n", __func__, c->inject_data.retry_counter, c->logical_pos);
        ret = airstashhttphook_reconnect_at(h, c->logical_pos);
        av_log(h, AV_LOG_INFO, "%s: did reconnect(%d) at %"PRId64": %d\n", __func__, c->inject_data.retry_counter, c->logical_pos, ret);
        if (ret)
            continue;

        read_ret = airstashurlhook_read(h, buf, size);
    }

fail:
    return read_ret;
}

static int64_t airstashhttphook_reseek_at(URLContext *h, int64_t pos, int whence, int force_reconnect)
{
    Context *c = h->priv_data;
    int ret = 0;

    if (!force_reconnect)
        return airstashurlhook_seek(h, pos, whence);

    if (whence == SEEK_CUR)
        pos += c->logical_pos;
    else if (whence == SEEK_END)
        pos += c->logical_size;
    else if (whence != SEEK_SET)
        return AVERROR(EINVAL);
    if (pos < 0)
        return AVERROR(EINVAL);

    ret = airstashhttphook_reconnect_at(h, pos);
    if (ret) {
        c->io_error = ret;
        return ret;
    }

    c->io_error = 0;
    return c->logical_pos;
}

static int64_t airstashhttphook_seek(URLContext *h, int64_t pos, int whence)
{
    Context *c = h->priv_data;
    int     ret      = 0;
    int64_t seek_ret = -1;

    if (whence == AVSEEK_SIZE)
        return c->logical_size;
    else if ((whence == SEEK_CUR && pos == 0) ||
             (whence == SEEK_SET && pos == c->logical_pos))
        return c->logical_pos;
    else if ((c->logical_size < 0 && whence == SEEK_END) || h->is_streamed)
        return AVERROR(ENOSYS);

    c->inject_data.retry_counter = 0;
    ret = airstashurlhook_call_inject(h);
    if (ret) {
        ret = AVERROR_EXIT;
        goto fail;
    }

    seek_ret = airstashhttphook_reseek_at(h, pos, whence, c->inject_data.is_url_changed);
    while (seek_ret < 0) {
        switch (seek_ret) {
            case AVERROR_EXIT:
            case AVERROR_EOF:
                goto fail;
        }

        c->inject_data.retry_counter++;
        ret = airstashurlhook_call_inject(h);
        if (ret) {
            ret = AVERROR_EXIT;
            goto fail;
        }

        if (!c->inject_data.is_handled)
            goto fail;

        av_log(h, AV_LOG_INFO, "%s: will reseek(%d) at pos=%"PRId64", whence=%d\n", __func__, c->inject_data.retry_counter, pos, whence);
        seek_ret = airstashhttphook_reseek_at(h, pos, whence, c->inject_data.is_url_changed);
        av_log(h, AV_LOG_INFO, "%s: did reseek(%d) at pos=%"PRId64", whence=%d: %"PRId64"\n", __func__, c->inject_data.retry_counter, pos, whence, seek_ret);
    }

    if (c->test_fail_point)
        c->test_fail_point_next = c->logical_pos + c->test_fail_point;
    c->io_error = 0;
    return c->logical_pos;
fail:
    return ret;
}

#define OFFSET(x) offsetof(Context, x)
#define D AV_OPT_FLAG_DECODING_PARAM

static const AVOption airstashtcphook_options[] = {
    { "airstashinject-opaque",               "private data of user, passed with custom callback",
        OFFSET(opaque),                 AIRSTASHAV_OPTION_INT64(0, INT64_MIN, INT64_MAX) },
    { "airstashinject-segment-index",        "segment index of current url",
        OFFSET(segment_index),          AIRSTASHAV_OPTION_INT(0, 0, INT_MAX) },
    { "airstashtcphook-test-fail-point",     "test fail point, in bytes",
        OFFSET(test_fail_point),        AIRSTASHAV_OPTION_INT(0, 0, INT_MAX) },
    { NULL }
};

static const AVOption airstashhttphook_options[] = {
    { "airstashinject-opaque",               "private data of user, passed with custom callback",
        OFFSET(opaque),                 AIRSTASHAV_OPTION_INT64(0, INT64_MIN, INT64_MAX) },
    { "airstashinject-segment-index",        "segment index of current url",
        OFFSET(segment_index),          AIRSTASHAV_OPTION_INT(0, 0, INT_MAX) },
    { "airstashhttphook-test-fail-point",    "test fail point, in bytes",
        OFFSET(test_fail_point),        AIRSTASHAV_OPTION_INT(0, 0, INT_MAX) },
    { NULL }
};

#undef D
#undef OFFSET

static const AVClass airstashtcphook_context_class = {
    .class_name = "TcpHook",
    .item_name  = av_default_item_name,
    .option     = airstashtcphook_options,
    .version    = LIBAVUTIL_VERSION_INT,
};

URLProtocol airstashff_airstashtcphook_protocol = {
    .name                = "airstashtcphook",
    .url_open2           = airstashtcphook_open,
    .url_read            = airstashurlhook_read,
    .url_write           = airstashurlhook_write,
    .url_close           = airstashurlhook_close,
    .priv_data_size      = sizeof(Context),
    .priv_data_class     = &airstashtcphook_context_class,
};

static const AVClass airstashhttphook_context_class = {
    .class_name = "HttpHook",
    .item_name  = av_default_item_name,
    .option     = airstashhttphook_options,
    .version    = LIBAVUTIL_VERSION_INT,
};

URLProtocol airstashff_airstashhttphook_protocol = {
    .name                = "airstashhttphook",
    .url_open2           = airstashhttphook_open,
    .url_read            = airstashhttphook_read,
    .url_write           = airstashurlhook_write,
    .url_seek            = airstashhttphook_seek,
    .url_close           = airstashurlhook_close,
    .priv_data_size      = sizeof(Context),
    .priv_data_class     = &airstashhttphook_context_class,
};
