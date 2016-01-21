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

#include "meta.h"
#include "ff_ffinc.h"
#include "sdl/sdl_misc.h"

#define AIRSTASH_META_INIT_CAPACITY 13

struct AirStashMediaMeta {
    SDL_mutex *mutex;

    AVDictionary *dict;

    size_t children_count;
    size_t children_capacity;
    AirStashMediaMeta **children;
};

AirStashMediaMeta *airstashmeta_create()
{
    AirStashMediaMeta *meta = (AirStashMediaMeta *)calloc(1, sizeof(AirStashMediaMeta));
    if (!meta)
        return NULL;

    meta->mutex = SDL_CreateMutex();
    if (!meta->mutex)
        goto fail;

    return meta;
fail:
    airstashmeta_destroy(meta);
    return NULL;
}

void airstashmeta_reset(AirStashMediaMeta *meta)
{
    if (meta && meta->dict)
        av_dict_free(&meta->dict);
}

void airstashmeta_destroy(AirStashMediaMeta *meta)
{
    if (!meta)
        return;

    if (meta->dict) {
        av_dict_free(&meta->dict);
    }

    if (meta->children) {
        for(int i = 0; i < meta->children_count; ++i) {
            AirStashMediaMeta *child = meta->children[i];
            if (child) {
                airstashmeta_destroy(child);
            }
        }
        free(meta->children);
        meta->children = NULL;
    }

    SDL_DestroyMutexP(&meta->mutex);
    free(meta);
}

void airstashmeta_destroy_p(AirStashMediaMeta **meta)
{
    if (!meta)
        return;

    airstashmeta_destroy(*meta);
    *meta = NULL;
}

void airstashmeta_lock(AirStashMediaMeta *meta)
{
    if (!meta || !meta->mutex)
        return;

    SDL_LockMutex(meta->mutex);
}

void airstashmeta_unlock(AirStashMediaMeta *meta)
{
    if (!meta || !meta->mutex)
        return;

    SDL_UnlockMutex(meta->mutex);
}

void airstashmeta_append_child_l(AirStashMediaMeta *meta, AirStashMediaMeta *child)
{
    if (!meta || !child)
        return;

    if (!meta->children) {
        meta->children = (AirStashMediaMeta **)calloc(AIRSTASH_META_INIT_CAPACITY, sizeof(AirStashMediaMeta *));
        if (!meta->children)
            return;
        meta->children_count    = 0;
        meta->children_capacity = AIRSTASH_META_INIT_CAPACITY;
    } else if (meta->children_count >= meta->children_capacity) {
        size_t new_capacity = meta->children_capacity * 2;
        AirStashMediaMeta **new_children = (AirStashMediaMeta **)calloc(new_capacity, sizeof(AirStashMediaMeta *));
        if (!new_children)
            return;

        memcpy(new_children, meta->children, meta->children_capacity * sizeof(AirStashMediaMeta *));
        free(meta->children);
        meta->children          = new_children;
        meta->children_capacity = new_capacity;
    }

    meta->children[meta->children_count] = child;
    meta->children_count++;
}

void airstashmeta_set_int64_l(AirStashMediaMeta *meta, const char *name, int64_t value)
{
    if (!meta)
        return;

    av_dict_set_int(&meta->dict, name, value, 0);
}

void airstashmeta_set_string_l(AirStashMediaMeta *meta, const char *name, const char *value)
{
    if (!meta)
        return;

    av_dict_set(&meta->dict, name, value, 0);
}

static int get_bit_rate(AVCodecContext *ctx)
{
    int bit_rate;
    int bits_per_sample;

    switch (ctx->codec_type) {
        case AVMEDIA_TYPE_VIDEO:
        case AVMEDIA_TYPE_DATA:
        case AVMEDIA_TYPE_SUBTITLE:
        case AVMEDIA_TYPE_ATTACHMENT:
            bit_rate = ctx->bit_rate;
            break;
        case AVMEDIA_TYPE_AUDIO:
            bits_per_sample = av_get_bits_per_sample(ctx->codec_id);
            bit_rate = bits_per_sample ? ctx->sample_rate * ctx->channels * bits_per_sample : ctx->bit_rate;
            break;
        default:
            bit_rate = 0;
            break;
    }
    return bit_rate;
}

void airstashmeta_set_avformat_context_l(AirStashMediaMeta *meta, AVFormatContext *ic)
{
    if (!meta || !ic)
        return;

    if (ic->iformat && ic->iformat->name)
        airstashmeta_set_string_l(meta, AIRSTASHM_KEY_FORMAT, ic->iformat->name);

    if (ic->duration != AV_NOPTS_VALUE)
        airstashmeta_set_int64_l(meta, AIRSTASHM_KEY_DURATION_US, ic->duration);

    if (ic->start_time != AV_NOPTS_VALUE)
        airstashmeta_set_int64_l(meta, AIRSTASHM_KEY_START_US, ic->start_time);

    if (ic->bit_rate)
        airstashmeta_set_int64_l(meta, AIRSTASHM_KEY_BITRATE, ic->bit_rate);

    AirStashMediaMeta *stream_meta = NULL;
    for (int i = 0; i < ic->nb_streams; i++) {
        if (!stream_meta)
            airstashmeta_destroy_p(&stream_meta);

        AVStream *st = ic->streams[i];
        if (!st || !st->codec)
            continue;

        stream_meta = airstashmeta_create();
        if (!stream_meta)
            continue;

        AVCodecContext *avctx = st->codec;
        const char *codec_name = avcodec_get_name(avctx->codec_id);
        if (codec_name)
            airstashmeta_set_string_l(stream_meta, AIRSTASHM_KEY_CODEC_NAME, codec_name);
        if (avctx->profile != FF_PROFILE_UNKNOWN) {
            const AVCodec *codec = avctx->codec ? avctx->codec : avcodec_find_decoder(avctx->codec_id);
            if (codec) {
                const char *profile = av_get_profile_name(codec, avctx->profile);
                if (profile)
                    airstashmeta_set_string_l(stream_meta, AIRSTASHM_KEY_CODEC_PROFILE, profile);
                if (codec->long_name)
                    airstashmeta_set_string_l(stream_meta, AIRSTASHM_KEY_CODEC_LONG_NAME, codec->long_name);
                airstashmeta_set_int64_l(stream_meta, AIRSTASHM_KEY_CODEC_LEVEL, avctx->level);
                if (avctx->pix_fmt != AV_PIX_FMT_NONE)
                    airstashmeta_set_string_l(stream_meta, AIRSTASHM_KEY_CODEC_PIXEL_FORMAT, av_get_pix_fmt_name(avctx->pix_fmt));
            }
        }

        int64_t bitrate = get_bit_rate(avctx);
        if (bitrate > 0) {
            airstashmeta_set_int64_l(stream_meta, AIRSTASHM_KEY_BITRATE, bitrate);
        }

        switch (avctx->codec_type) {
            case AVMEDIA_TYPE_VIDEO: {
                airstashmeta_set_string_l(stream_meta, AIRSTASHM_KEY_TYPE, AIRSTASHM_VAL_TYPE__VIDEO);

                if (avctx->width > 0)
                    airstashmeta_set_int64_l(stream_meta, AIRSTASHM_KEY_WIDTH, avctx->width);
                if (avctx->height > 0)
                    airstashmeta_set_int64_l(stream_meta, AIRSTASHM_KEY_HEIGHT, avctx->height);
                if (st->sample_aspect_ratio.num > 0 && st->sample_aspect_ratio.den > 0) {
                    airstashmeta_set_int64_l(stream_meta, AIRSTASHM_KEY_SAR_NUM, avctx->sample_aspect_ratio.num);
                    airstashmeta_set_int64_l(stream_meta, AIRSTASHM_KEY_SAR_DEN, avctx->sample_aspect_ratio.den);
                }

                if (st->avg_frame_rate.num > 0 && st->avg_frame_rate.den > 0) {
                    airstashmeta_set_int64_l(stream_meta, AIRSTASHM_KEY_FPS_NUM, st->avg_frame_rate.num);
                    airstashmeta_set_int64_l(stream_meta, AIRSTASHM_KEY_FPS_DEN, st->avg_frame_rate.den);
                }
                if (st->r_frame_rate.num > 0 && st->r_frame_rate.den > 0) {
                    airstashmeta_set_int64_l(stream_meta, AIRSTASHM_KEY_TBR_NUM, st->avg_frame_rate.num);
                    airstashmeta_set_int64_l(stream_meta, AIRSTASHM_KEY_TBR_DEN, st->avg_frame_rate.den);
                }
                break;
            }
            case AVMEDIA_TYPE_AUDIO: {
                airstashmeta_set_string_l(stream_meta, AIRSTASHM_KEY_TYPE, AIRSTASHM_VAL_TYPE__AUDIO);

                if (avctx->sample_rate)
                    airstashmeta_set_int64_l(stream_meta, AIRSTASHM_KEY_SAMPLE_RATE, avctx->sample_rate);
                if (avctx->channel_layout)
                    airstashmeta_set_int64_l(stream_meta, AIRSTASHM_KEY_CHANNEL_LAYOUT, avctx->channel_layout);
                break;
            }
            default: {
                airstashmeta_set_string_l(stream_meta, AIRSTASHM_KEY_TYPE, AIRSTASHM_VAL_TYPE__UNKNOWN);
                break;
            }
        }

        AVDictionaryEntry *lang = av_dict_get(st->metadata, "language", NULL, 0);
        if (lang && lang->value)
            airstashmeta_set_string_l(stream_meta, AIRSTASHM_KEY_LANGUAGE, lang->value);

        airstashmeta_append_child_l(meta, stream_meta);
        stream_meta = NULL;
    }

    if (!stream_meta)
        airstashmeta_destroy_p(&stream_meta);
}

const char *airstashmeta_get_string_l(AirStashMediaMeta *meta, const char *name)
{
    if (!meta || !meta->dict)
        return NULL;

    AVDictionaryEntry *entry = av_dict_get(meta->dict, name, NULL, 0);
    if (!entry)
        return NULL;

    return entry->value;
}

int64_t airstashmeta_get_int64_l(AirStashMediaMeta *meta, const char *name, int64_t defaultValue)
{
    if (!meta || !meta->dict)
        return defaultValue;

    AVDictionaryEntry *entry = av_dict_get(meta->dict, name, NULL, 0);
    if (!entry || !entry->value)
        return defaultValue;

    return atoll(entry->value);
}

size_t airstashmeta_get_children_count_l(AirStashMediaMeta *meta)
{
    if (!meta || !meta->children)
        return 0;

    return meta->children_count;
}

AirStashMediaMeta *airstashmeta_get_child_l(AirStashMediaMeta *meta, size_t index)
{
    if (!meta)
        return NULL;

    if (index >= meta->children_count)
        return NULL;

    return meta->children[index];
}
