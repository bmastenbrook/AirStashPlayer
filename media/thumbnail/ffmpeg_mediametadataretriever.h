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


#ifndef FFMPEG_MEDIAMETADATARETRIEVER_H_
#define FFMPEG_MEDIAMETADATARETRIEVER_H_

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/dict.h>

typedef enum {
    OPTION_PREVIOUS_SYNC = 0,
    OPTION_NEXT_SYNC = 1,
    OPTION_CLOSEST_SYNC = 2,
    OPTION_CLOSEST = 3,
    
    // Add more here...
} Options;

typedef struct State {
    AVFormatContext *pFormatCtx;
    int             audio_stream;
    int             video_stream;
    AVStream        *audio_st;
    AVStream        *video_st;
    int             fd;
    int64_t         offset;
    const char      *headers;
} State;

struct AVDictionary {
    int count;
    AVDictionaryEntry *elems;
};

int set_data_source_uri(State **ps, const char* path, const char* headers);
int set_data_source_fd(State **ps, int fd, int64_t offset, int64_t length);
const char* extract_metadata(State **ps, const char* key);
const char* extract_metadata_from_chapter(State **ps, const char* key, int chapter);
int get_metadata(State **ps, AVDictionary **metadata);
int get_embedded_picture(State **ps, AVPacket *pkt);
int get_frame_at_time(State **ps, int64_t timeUs, int option, AVPacket *pkt);
int get_scaled_frame_at_time(State **ps, int64_t timeUs, int option, AVPacket *pkt, int width, int height);
void release(State **ps);

#endif /*FFMPEG_MEDIAMETADATARETRIEVER_H_*/