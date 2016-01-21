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

#ifndef AIRSTASHPLAYER__META_H
#define AIRSTASHPLAYER__META_H

#include <stdint.h>
#include <stdlib.h>

// media meta
#define AIRSTASHM_KEY_FORMAT         "format"
#define AIRSTASHM_KEY_DURATION_US    "duration_us"
#define AIRSTASHM_KEY_START_US       "start_us"
#define AIRSTASHM_KEY_BITRATE        "bitrate"
#define AIRSTASHM_KEY_VIDEO_STREAM   "video"
#define AIRSTASHM_KEY_AUDIO_STREAM   "audio"

// stream meta
#define AIRSTASHM_KEY_TYPE           "type"
#define AIRSTASHM_VAL_TYPE__VIDEO    "video"
#define AIRSTASHM_VAL_TYPE__AUDIO    "audio"
#define AIRSTASHM_VAL_TYPE__UNKNOWN  "unknown"
#define AIRSTASHM_KEY_LANGUAGE       "language"

#define AIRSTASHM_KEY_CODEC_NAME         "codec_name"
#define AIRSTASHM_KEY_CODEC_PROFILE      "codec_profile"
#define AIRSTASHM_KEY_CODEC_LEVEL        "codec_level"
#define AIRSTASHM_KEY_CODEC_LONG_NAME    "codec_long_name"
#define AIRSTASHM_KEY_CODEC_PIXEL_FORMAT "codec_pixel_format"

// stream: video
#define AIRSTASHM_KEY_WIDTH          "width"
#define AIRSTASHM_KEY_HEIGHT         "height"
#define AIRSTASHM_KEY_FPS_NUM        "fps_num"
#define AIRSTASHM_KEY_FPS_DEN        "fps_den"
#define AIRSTASHM_KEY_TBR_NUM        "tbr_num"
#define AIRSTASHM_KEY_TBR_DEN        "tbr_den"
#define AIRSTASHM_KEY_SAR_NUM        "sar_num"
#define AIRSTASHM_KEY_SAR_DEN        "sar_den"
// stream: audio
#define AIRSTASHM_KEY_SAMPLE_RATE    "sample_rate"
#define AIRSTASHM_KEY_CHANNEL_LAYOUT "channel_layout"

// reserved for user
#define AIRSTASHM_KEY_STREAMS        "streams"

struct AVFormatContext;
typedef struct AirStashMediaMeta AirStashMediaMeta;

AirStashMediaMeta *airstashmeta_create();
void airstashmeta_reset(AirStashMediaMeta *meta);
void airstashmeta_destroy(AirStashMediaMeta *meta);
void airstashmeta_destroy_p(AirStashMediaMeta **meta);

void airstashmeta_lock(AirStashMediaMeta *meta);
void airstashmeta_unlock(AirStashMediaMeta *meta);

void airstashmeta_append_child_l(AirStashMediaMeta *meta, AirStashMediaMeta *child);
void airstashmeta_set_int64_l(AirStashMediaMeta *meta, const char *name, int64_t value);
void airstashmeta_set_string_l(AirStashMediaMeta *meta, const char *name, const char *value);
void airstashmeta_set_avformat_context_l(AirStashMediaMeta *meta, struct AVFormatContext *ic);

// must be freed with free();
const char   *airstashmeta_get_string_l(AirStashMediaMeta *meta, const char *name);
int64_t       airstashmeta_get_int64_l(AirStashMediaMeta *meta, const char *name, int64_t defaultValue);
size_t        airstashmeta_get_children_count_l(AirStashMediaMeta *meta);
// do not free
AirStashMediaMeta *airstashmeta_get_child_l(AirStashMediaMeta *meta, size_t index);

#endif//AIRSTASHPLAYER__META_H
