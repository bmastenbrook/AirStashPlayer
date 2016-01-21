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
    char           *inner_url;

    /* options */
    int64_t         opaque;
    int             segment_index;
} Context;

#if 0
static void *airstashinject_get_opaque(URLContext *h) {
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
#endif

static int airstashinject_open(URLContext *h, const char *arg, int flags, AVDictionary **options)
{
    Context *c = h->priv_data;
    int ret = -1;

    av_strstart(arg, "airstashinject:", &arg);

    c->inner_url = av_strdup(arg);

    av_dict_set_int(options, "airstashinject-opaque",        c->opaque, 0);
    av_dict_set_int(options, "airstashinject-segment-index", c->segment_index, 0);
    ret = ffurl_open(&c->inner, arg, flags, &h->interrupt_callback, options);
    if (ret)
        goto fail;

    return ret;
fail:
    av_freep(&c->inner_url);
    return ret;
}

static int airstashinject_close(URLContext *h)
{
    Context *c = h->priv_data;

    av_freep(&c->inner_url);

    return ffurl_close(c->inner);
}

static int airstashinject_read(URLContext *h, unsigned char *buf, int size)
{
    Context *c = h->priv_data;

    return ffurl_read(c->inner, buf, size);
}

static int64_t airstashinject_seek(URLContext *h, int64_t pos, int whence)
{
    Context *c = h->priv_data;

    return ffurl_seek(c->inner, pos, whence);
}

#define OFFSET(x) offsetof(Context, x)
#define D AV_OPT_FLAG_DECODING_PARAM

static const AVOption options[] = {
    { "airstashinject-opaque",           "private data of user, passed with custom callback",
        OFFSET(opaque),             AIRSTASHAV_OPTION_INT64(0, INT64_MIN, INT64_MAX) },
    { NULL }
};

#undef D
#undef OFFSET

static const AVClass airstashinject_context_class = {
    .class_name = "Inject",
    .item_name  = av_default_item_name,
    .option     = options,
    .version    = LIBAVUTIL_VERSION_INT,
};

URLProtocol airstashff_airstashinject_protocol = {
    .name                = "airstashinject",
    .url_open2           = airstashinject_open,
    .url_read            = airstashinject_read,
    .url_seek            = airstashinject_seek,
    .url_close           = airstashinject_close,
    .priv_data_size      = sizeof(Context),
    .priv_data_class     = &airstashinject_context_class,
};
