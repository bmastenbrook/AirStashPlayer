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

#ifndef AIRSTASH__android_media_PlaybackParams__H
#define AIRSTASH__android_media_PlaybackParams__H

#include "sdl/android/airstash/internal/internal.h"

jobject AirStashC_PlaybackParams__setSpeed(JNIEnv *env, jobject thiz, jfloat speed);
jobject AirStashC_PlaybackParams__setSpeed__catchAll(JNIEnv *env, jobject thiz, jfloat speed);
jobject AirStashC_PlaybackParams__setSpeed__asGlobalRef__catchAll(JNIEnv *env, jobject thiz, jfloat speed);
int AirStash_loadClass__AirStashC_PlaybackParams(JNIEnv *env);

#define AIRSTASH_HAVE__AirStashC_PlaybackParams

#endif//AIRSTASH__android_media_PlaybackParams__H
