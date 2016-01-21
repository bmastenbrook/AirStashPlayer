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
    char           *url;
} Context;

static int airstashlongurl_open(URLContext *h, const char *arg, int flags, AVDictionary **options)
{
    Context *c = h->priv_data;

    if (!c->url || !*c->url)
        return AVERROR_EXTERNAL;

    return ffurl_open(&c->inner, c->url, flags, &h->interrupt_callback, options);
}

static int airstashlongurl_close(URLContext *h)
{
    Context *c = h->priv_data;

    return ffurl_close(c->inner);
}

static int airstashlongurl_read(URLContext *h, unsigned char *buf, int size)
{
    Context *c = h->priv_data;

    return ffurl_read(c->inner, buf, size);
}

static int64_t airstashlongurl_seek(URLContext *h, int64_t pos, int whence)
{
    Context *c = h->priv_data;

    return ffurl_seek(c->inner, pos, whence);
}

#define OFFSET(x) offsetof(Context, x)
#define D AV_OPT_FLAG_DECODING_PARAM

static const AVOption options[] = {
    { "airstashlongurl-url",         "real url to access",
        OFFSET(url),            AIRSTASHAV_OPTION_STR(NULL) },
    { NULL }
};

#undef D
#undef OFFSET

static const AVClass airstashlongurl_context_class = {
    .class_name = "LongUrl",
    .item_name  = av_default_item_name,
    .option     = options,
    .version    = LIBAVUTIL_VERSION_INT,
};

URLProtocol airstashff_airstashlongurl_protocol = {
    .name                = "airstashlongurl",
    .url_open2           = airstashlongurl_open,
    .url_read            = airstashlongurl_read,
    .url_seek            = airstashlongurl_seek,
    .url_close           = airstashlongurl_close,
    .priv_data_size      = sizeof(Context),
    .priv_data_class     = &airstashlongurl_context_class,
};
