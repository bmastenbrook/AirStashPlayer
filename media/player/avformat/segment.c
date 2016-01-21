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

    /* options */
    int64_t         opaque;
    int             segment_index;
    char           *http_hook;
} Context;

static void *airstashsegment_get_opaque(URLContext *h) {
    Context *c = h->priv_data;
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#endif
    return (void *)c->opaque;
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
}

static int airstashsegment_open(URLContext *h, const char *arg, int flags, AVDictionary **options)
{
    Context *c = h->priv_data;
    AirStashAVInject_OnUrlOpenData inject_data = {0};
    AirStashAVInjectCallback inject_callback = airstashav_get_inject_callback();
    int ret = -1;
    void *opaque = airstashsegment_get_opaque(h);
    assert(opaque);

    if (!c->opaque) {
        av_log(h, AV_LOG_ERROR, "null opaque\n");
        return AVERROR_EXTERNAL;
    }

    if (!inject_callback) {
        av_log(h, AV_LOG_ERROR, "null inject_callback\n");
        return AVERROR_EXTERNAL;
    }

    av_strstart(arg, "airstashsegment:", &arg);
    if (!arg || !*arg)
        return AVERROR_EXTERNAL;

    inject_data.size = sizeof(inject_data);
    inject_data.segment_index = (int)strtol(arg, NULL, 0);
    strlcpy(inject_data.url,    arg,    sizeof(inject_data.url));

    if (opaque && inject_callback &&
        inject_data.segment_index < 0) {
        ret = AVERROR_EXTERNAL;
        goto fail;
    }

    ret = inject_callback(opaque, AIRSTASHAVINJECT_CONCAT_RESOLVE_SEGMENT, &inject_data, sizeof(inject_data));
    if (ret || !inject_data.url[0]) {
        ret = AVERROR_EXIT;
        goto fail;
    }

    av_dict_set_int(options, "airstashinject-opaque",        c->opaque, 0);
    av_dict_set_int(options, "airstashinject-segment-index", c->segment_index, 0);

    ret = ffurl_open(&c->inner, inject_data.url, flags, &h->interrupt_callback, options);
    if (ret)
        goto fail;

    return 0;
fail:
    return ret;
}

static int airstashsegment_close(URLContext *h)
{
    Context *c = h->priv_data;

    return ffurl_close(c->inner);
}

static int airstashsegment_read(URLContext *h, unsigned char *buf, int size)
{
    Context *c = h->priv_data;

    return ffurl_read(c->inner, buf, size);
}

static int64_t airstashsegment_seek(URLContext *h, int64_t pos, int whence)
{
    Context *c = h->priv_data;

    return ffurl_seek(c->inner, pos, whence);
}

#define OFFSET(x) offsetof(Context, x)
#define D AV_OPT_FLAG_DECODING_PARAM

static const AVOption options[] = {
    { "airstashinject-opaque",           "private data of user, passed with custom callback",
        OFFSET(opaque),             AIRSTASHAV_OPTION_INT64(0, INT64_MIN, INT64_MAX) },
    { "airstashinject-segment-index",    "segment index of current url",
        OFFSET(segment_index),      AIRSTASHAV_OPTION_INT(0, 0, INT_MAX) },
    { NULL }
};

#undef D
#undef OFFSET

static const AVClass airstashsegment_context_class = {
    .class_name = "Inject",
    .item_name  = av_default_item_name,
    .option     = options,
    .version    = LIBAVUTIL_VERSION_INT,
};

URLProtocol airstashff_airstashsegment_protocol = {
    .name                = "airstashsegment",
    .url_open2           = airstashsegment_open,
    .url_read            = airstashsegment_read,
    .url_seek            = airstashsegment_seek,
    .url_close           = airstashsegment_close,
    .priv_data_size      = sizeof(Context),
    .priv_data_class     = &airstashsegment_context_class,
};
