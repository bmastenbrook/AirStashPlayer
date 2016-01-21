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
 * AirStashMetadataRetriever is based upon FFmpegMediaMetadataRetriever:
 * Copyright 2015 William Seemann
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


#ifndef FFMPEG_UTILS_H_
#define FFMPEG_UTILS_H_

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/opt.h>

static const char *DURATION = "duration";
static const char *AUDIO_CODEC = "audio_codec";
static const char *VIDEO_CODEC = "video_codec";
static const char *ICY_METADATA = "icy_metadata";
static const char *ROTATE = "rotate";
static const char *FRAMERATE = "framerate";
static const char *CHAPTER_START_TIME = "chapter_start_time";
static const char *CHAPTER_END_TIME = "chapter_end_time";
static const char *CHAPTER_COUNT = "chapter_count";
static const char *FILESIZE = "filesize";


static const int SUCCESS = 0;
static const int FAILURE = -1;

void set_shoutcast_metadata(AVFormatContext *ic);
void set_duration(AVFormatContext *ic);
void set_codec(AVFormatContext *ic, int i);
void set_rotation(AVFormatContext *ic, AVStream *audio_st, AVStream *video_st);
void set_framerate(AVFormatContext *ic, AVStream *audio_st, AVStream *video_st);
void set_filesize(AVFormatContext *ic);
void set_chapter_count(AVFormatContext *ic);
const char* extract_metadata_internal(AVFormatContext *ic, AVStream *audio_st, AVStream *video_st, const char* key);
int get_metadata_internal(AVFormatContext *ic, AVDictionary **metadata);
const char* extract_metadata_from_chapter_internal(AVFormatContext *ic, AVStream *audio_st, AVStream *video_st, const char* key, int chapter);

#endif /*FFMPEG_UTILS_H_*/