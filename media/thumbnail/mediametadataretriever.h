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

#ifndef MEDIAMETADATARETRIEVER_H
#define MEDIAMETADATARETRIEVER_H

extern "C" {
#include "ffmpeg_mediametadataretriever.h"
}

class MediaMetadataRetriever
{
    State* state;
public:
    MediaMetadataRetriever();
    ~MediaMetadataRetriever();
    void disconnect();
    int setDataSource(const char* dataSourceUrl, const char* headers);
    int setDataSource(int fd, int64_t offset, int64_t length);
    int getFrameAtTime(int64_t timeUs, int option, AVPacket *pkt);
    int getScaledFrameAtTime(int64_t timeUs, int option, AVPacket *pkt, int width, int height);
    int extractAlbumArt(AVPacket *pkt);
    const char* extractMetadata(const char* key);
    const char* extractMetadataFromChapter(const char* key, int chapter);
    int getMetadata(bool update_only, bool apply_filter, AVDictionary **metadata);
};

#endif // MEDIAMETADATARETRIEVER_H