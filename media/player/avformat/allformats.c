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

#include "libavformat/avformat.h"
#include "libavformat/url.h"
#include "libavformat/version.h"

#define FF_REGISTER_PROTOCOL(x)                                         \
    {                                                                   \
        extern URLProtocol ff_##x##_protocol;                           \
        airstashav_register_protocol(&ff_##x##_protocol);                    \
    }

#define AIRSTASH_REGISTER_PROTOCOL(x)                                        \
    {                                                                   \
        extern URLProtocol airstashff_##x##_protocol;                        \
        airstashav_register_protocol(&airstashff_##x##_protocol);                 \
    }

#define AIRSTASH_REGISTER_DEMUXER(x)                                         \
    {                                                                   \
        extern AVInputFormat airstashff_##x##_demuxer;                       \
        airstashav_register_input_format(&airstashff_##x##_demuxer);              \
    }


static struct URLProtocol *airstashav_find_protocol(const char *proto_name)
{
    URLProtocol *up = NULL;
    if (!proto_name)
        return NULL;
    while ((up = ffurl_protocol_next(up)) != NULL) {
        if (!up->name)
            continue;
        if (!strcmp(proto_name, up->name))
            break;
    }
    return up;
}

static struct AVInputFormat *airstashav_find_input_format(const char *iformat_name)
{
    AVInputFormat *fmt = NULL;
    if (!iformat_name)
        return NULL;
    while ((fmt = av_iformat_next(fmt))) {
        if (!fmt->name)
            continue;
        if (!strcmp(iformat_name, fmt->name))
            return fmt;
    }
    return NULL;
}

static void airstashav_register_protocol(URLProtocol *protocol)
{
    if (airstashav_find_protocol(protocol->name)) {
        av_log(NULL, AV_LOG_WARNING, "skip     protocol: %s (duplicated)\n", protocol->name);
    } else {
        av_log(NULL, AV_LOG_INFO,    "register protocol: %s\n", protocol->name);
        ffurl_register_protocol(protocol);
    }
}

static void airstashav_register_input_format(AVInputFormat *iformat)
{
    if (airstashav_find_input_format(iformat->name)) {
        av_log(NULL, AV_LOG_WARNING, "skip     demuxer : %s (duplicated)\n", iformat->name);
    } else {
        av_log(NULL, AV_LOG_INFO,    "register demuxer : %s\n", iformat->name);
        av_register_input_format(iformat);
    }
}

void airstashav_register_all(void)
{
    static int initialized;

    if (initialized)
        return;
    initialized = 1;

    av_register_all();

    /* protocols */
    av_log(NULL, AV_LOG_INFO, "===== custom modules begin =====\n");
    FF_REGISTER_PROTOCOL(async);
    AIRSTASH_REGISTER_PROTOCOL(airstashhttphook);
    AIRSTASH_REGISTER_PROTOCOL(airstashinject);
    AIRSTASH_REGISTER_PROTOCOL(airstashlongurl);
#ifdef __ANDROID__
    AIRSTASH_REGISTER_PROTOCOL(airstashmediadatasource);
#endif
    AIRSTASH_REGISTER_PROTOCOL(airstashsegment);
    AIRSTASH_REGISTER_PROTOCOL(airstashtcphook);

    /* demuxers */
    AIRSTASH_REGISTER_DEMUXER(airstashlivehook);
    av_log(NULL, AV_LOG_INFO, "===== custom modules end =====\n");
}
