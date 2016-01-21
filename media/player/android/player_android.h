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

#ifndef AIRSTASHPLAYER_ANDROID__AIRSTASHPLAYER_ANDROID_H
#define AIRSTASHPLAYER_ANDROID__AIRSTASHPLAYER_ANDROID_H

#include <jni.h>
#include "player_android_def.h"
#include "../player.h"

typedef struct airstashmp_android_media_format_context {
    const char *mime_type;
    int         profile;
    int         level;
} airstashmp_android_media_format_context;

// ref_count is 1 after open
AirStashMediaPlayer *airstashmp_android_create(int(*msg_loop)(void*));

void airstashmp_android_set_surface(JNIEnv *env, AirStashMediaPlayer *mp, jobject android_surface);
void airstashmp_android_set_volume(JNIEnv *env, AirStashMediaPlayer *mp, float left, float right);
int  airstashmp_android_get_audio_session_id(JNIEnv *env, AirStashMediaPlayer *mp);
void airstashmp_android_set_mediacodec_select_callback(AirStashMediaPlayer *mp, bool (*callback)(void *opaque, airstashmp_mediacodecinfo_context *mcc), void *opaque);

#endif
