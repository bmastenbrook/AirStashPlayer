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
#include "libavutil/avstring.h"
#include "libavutil/opt.h"

#include "player/avutil/opt.h"
#include "avformat.h"

typedef struct {
    AVClass         *class;
    AVFormatContext *inner;

    AirStashAVInject_OnUrlOpenData inject_data;
    int              discontinuity;
    int              error;

    /* options */
    AVDictionary   *open_opts;
    int64_t         opaque;
} Context;

static int airstashurlhook_call_inject(AVFormatContext *h)
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
        av_log(h, AV_LOG_INFO, "livehook %s\n", c->inject_data.url);
        c->inject_data.is_handled = 0;
        ret = inject_callback(opaque, AIRSTASHAVINJECT_ON_LIVE_RETRY, &c->inject_data, sizeof(c->inject_data));
        if (ret || !c->inject_data.url[0]) {
            ret = AVERROR_EXIT;
            goto fail;
        }
    }

    if (ff_check_interrupt(&h->interrupt_callback)) {
        ret = AVERROR_EXIT;
        goto fail;
    }

fail:
    return ret;
}

static int airstashlivehook_probe(AVProbeData *probe)
{
    if (av_strstart(probe->filename, "airstashlivehook:", NULL))
        return AVPROBE_SCORE_MAX;

    return 0;
}

static int airstashlivehook_read_close(AVFormatContext *avf)
{
    Context *c = avf->priv_data;

    avformat_close_input(&c->inner);
    return 0;
}

// FIXME: install libavformat/internal.h
int ff_alloc_extradata(AVCodecContext *avctx, int size);

static int copy_stream_props(AVStream *st, AVStream *source_st)
{
    int ret;

    if (st->codec->codec_id || !source_st->codec->codec_id) {
        if (st->codec->extradata_size < source_st->codec->extradata_size) {
            ret = ff_alloc_extradata(st->codec,
                                     source_st->codec->extradata_size);
            if (ret < 0)
                return ret;
        }
        memcpy(st->codec->extradata, source_st->codec->extradata,
               source_st->codec->extradata_size);
        return 0;
    }
    if ((ret = avcodec_copy_context(st->codec, source_st->codec)) < 0)
        return ret;
    st->r_frame_rate        = source_st->r_frame_rate;
    st->avg_frame_rate      = source_st->avg_frame_rate;
    st->time_base           = source_st->time_base;
    st->sample_aspect_ratio = source_st->sample_aspect_ratio;
    return 0;
}

static int open_inner(AVFormatContext *avf)
{
    Context         *c          = avf->priv_data;
    AVDictionary    *tmp_opts   = NULL;
    AVFormatContext *new_avf    = NULL;
    int ret = -1;
    int i   = 0;

    new_avf = avformat_alloc_context();
    if (!new_avf) {
        ret = AVERROR(ENOMEM);
        goto fail;
    }

    if (c->open_opts)
        av_dict_copy(&tmp_opts, c->open_opts, 0);

    new_avf->interrupt_callback = avf->interrupt_callback;
    ret = avformat_open_input(&new_avf, c->inject_data.url, NULL, &tmp_opts);
    if (ret < 0)
        goto fail;

    ret = avformat_find_stream_info(new_avf, NULL);
    if (ret < 0)
        goto fail;

    for (i = 0; i < new_avf->nb_streams; i++) {
        AVStream *st = avformat_new_stream(avf, NULL);
        if (!st) {
            ret = AVERROR(ENOMEM);
            goto fail;
        }

        ret = copy_stream_props(st, new_avf->streams[i]);
        if (ret < 0)
            goto fail;
    }

    avformat_close_input(&c->inner);
    c->inner = new_avf;
    new_avf = NULL;
    ret = 0;
fail:
    av_dict_free(&tmp_opts);
    avformat_close_input(&new_avf);
    return ret;
}

static int airstashlivehook_read_header(AVFormatContext *avf, AVDictionary **options)
{
    Context    *c           = avf->priv_data;
    const char *inner_url   = NULL;
    int         ret         = -1;

    av_strstart(avf->filename, "airstashlivehook:", &inner_url);

    c->inject_data.size = sizeof(c->inject_data);
    strlcpy(c->inject_data.url, inner_url, sizeof(c->inject_data.url));

    if (av_stristart(c->inject_data.url, "rtmp", NULL) ||
        av_stristart(c->inject_data.url, "rtsp", NULL)) {
        // There is total different meaning for 'timeout' option in rtmp
        av_log(avf, AV_LOG_WARNING, "remove 'timeout' option for rtmp.\n");
        av_dict_set(options, "timeout", NULL, 0);
    }

    if (options)
        av_dict_copy(&c->open_opts, *options, 0);

    c->inject_data.retry_counter = 0;
    ret = open_inner(avf);
    while (ret < 0) {
        // no EOF in live mode
        switch (ret) {
            case AVERROR_EXIT:
                goto fail;
        }

        c->inject_data.retry_counter++;
        ret = airstashurlhook_call_inject(avf);
        if (ret) {
            ret = AVERROR_EXIT;
            goto fail;
        }

        c->discontinuity = 1;
        ret = open_inner(avf);
    }

    return 0;
fail:
    return ret;
}

static int airstashlivehook_read_packet(AVFormatContext *avf, AVPacket *pkt)
{
    Context *c   = avf->priv_data;
    int      ret = -1;

    if (c->error)
        return c->error;

    if (c->inner)
        ret = av_read_frame(c->inner, pkt);

    c->inject_data.retry_counter = 0;
    while (ret < 0) {
        if (c->inner && c->inner->pb && c->inner->pb->error && avf->pb)
            avf->pb->error = c->inner->pb->error;

        // no EOF in live mode
        switch (ret) {
            case AVERROR_EXIT:
                c->error = AVERROR_EXIT;
                goto fail;
            case AVERROR(EAGAIN):
                goto continue_read;
        }

        c->inject_data.retry_counter++;
        ret = airstashurlhook_call_inject(avf);
        if (ret) {
            ret = AVERROR_EXIT;
            goto fail;
        }

        c->discontinuity = 1;
        ret = open_inner(avf);
        if (ret)
            continue;

continue_read:
        ret = av_read_frame(c->inner, pkt);
    }

    if (c->discontinuity) {
        pkt->flags |= AV_PKT_FLAG_DISCONTINUITY;
        c->discontinuity = 0;
    }

    return 0;
fail:
    return ret;
}

#define OFFSET(x) offsetof(Context, x)
#define D AV_OPT_FLAG_DECODING_PARAM

static const AVOption options[] = {
    { "airstashinject-opaque",       "private data of user, passed with custom callback",
        OFFSET(opaque),         AIRSTASHAV_OPTION_INT64(0, INT64_MIN, INT64_MAX) },
    { NULL }
};

#undef D
#undef OFFSET

static const AVClass airstashlivehook_class = {
    .class_name = "LiveHook demuxer",
    .item_name  = av_default_item_name,
    .option     = options,
    .version    = LIBAVUTIL_VERSION_INT,
};

AVInputFormat airstashff_airstashlivehook_demuxer = {
    .name           = "airstashlivehook",
    .long_name      = "Live Hook Controller",
    .flags          = AVFMT_NOFILE | AVFMT_TS_DISCONT,
    .priv_data_size = sizeof(Context),
    .read_probe     = airstashlivehook_probe,
    .read_header2   = airstashlivehook_read_header,
    .read_packet    = airstashlivehook_read_packet,
    .read_close     = airstashlivehook_read_close,
    .priv_class     = &airstashlivehook_class,
};
