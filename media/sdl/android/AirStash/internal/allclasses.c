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

#include "internal.h"
#include "sdl/android/AirStash/c/java/nio/ByteBuffer.h"
#include "sdl/android/AirStash/c/java/util/ArrayList.h"
#include "sdl/android/AirStash/c/android/os/Build.h"
#include "sdl/android/AirStash/c/android/media/AudioTrack.h"
#include "sdl/android/AirStash/c/android/media/MediaCodec.h"
#include "sdl/android/AirStash/c/android/media/MediaFormat.h"
#include "sdl/android/AirStash/c/android/media/PlaybackParams.h"
#include "sdl/android/AirStash/c/android/os/Bundle.h"
#include "sdl/android/AirStash/c/com/airstash/player/player/AirStashMediaPlayer.h"
#include "sdl/android/AirStash/c/com/airstash/player/player/misc/IMediaDataSource.h"

#define AIRSTASH_LOAD_CLASS(class__) \
    do { \
        ret = AirStash_loadClass__AirStashC_##class__(env); \
        if (ret) \
            goto fail; \
    } while (0)

int AirStash_LoadAll__catchAll(JNIEnv *env)
{
    int ret = 0;

    AIRSTASH_LOAD_CLASS(ByteBuffer); // java_nio_ByteBuffer
    AIRSTASH_LOAD_CLASS(ArrayList);  // java_util_ArrayList

    // must load before all other android classes
    AIRSTASH_LOAD_CLASS(android_os_Build);

    AIRSTASH_LOAD_CLASS(AudioTrack);     // android_media_AudioTrack
    AIRSTASH_LOAD_CLASS(MediaCodec);     // android_media_MediaCodec
    AIRSTASH_LOAD_CLASS(MediaFormat);    // android_media_MediaFormat
    AIRSTASH_LOAD_CLASS(PlaybackParams); // android_media_PlaybackParams
    AIRSTASH_LOAD_CLASS(Bundle);         // android_os_Bundle

    AIRSTASH_LOAD_CLASS(AirStashMediaPlayer);     // com_airstash_player_player_AirStashMediaPlayer
    AIRSTASH_LOAD_CLASS(IMediaDataSource);   // com_airstash_player_player_misc_IMediaDataSource

fail:
    return ret;
}
