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

#define LOG_TAG "AirStashMetadataRetriever"

#include <mediametadataretriever.h>

extern "C" {
#include "ffmpeg_mediametadataretriever.h"
}

MediaMetadataRetriever::MediaMetadataRetriever()
{
    state = NULL;
}

MediaMetadataRetriever::~MediaMetadataRetriever()
{
    ::release(&state);
}

int MediaMetadataRetriever::setDataSource(const char *srcUrl, const char *headers)
{
    return ::set_data_source_uri(&state, srcUrl, headers);
}

int MediaMetadataRetriever::setDataSource(int fd, int64_t offset, int64_t length)
{
    return ::set_data_source_fd(&state, fd, offset, length);
}

int MediaMetadataRetriever::getFrameAtTime(int64_t timeUs, int option, AVPacket *pkt)
{
    return ::get_frame_at_time(&state, timeUs, option, pkt);
}

int MediaMetadataRetriever::getScaledFrameAtTime(int64_t timeUs, int option, AVPacket *pkt, int width, int height)
{
    return ::get_scaled_frame_at_time(&state, timeUs, option, pkt, width, height);
}

const char* MediaMetadataRetriever::extractMetadata(const char *key)
{
    return ::extract_metadata(&state, key);
}

const char* MediaMetadataRetriever::extractMetadataFromChapter(const char *key, int chapter)
{
    return ::extract_metadata_from_chapter(&state, key, chapter);
}

int MediaMetadataRetriever::extractAlbumArt(AVPacket *pkt)
{
    return ::get_embedded_picture(&state, pkt);
}

int MediaMetadataRetriever::getMetadata(bool update_only, bool apply_filter, AVDictionary **metadata)
{
    return get_metadata(&state, metadata);
}