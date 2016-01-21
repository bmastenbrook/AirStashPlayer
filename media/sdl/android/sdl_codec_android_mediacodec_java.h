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

#ifndef SDL_ANDROID__ANDROID_CODEC_ANDROID_MEDIACODEC_JAVA_H
#define SDL_ANDROID__ANDROID_CODEC_ANDROID_MEDIACODEC_JAVA_H

#include "sdl_codec_android_mediacodec.h"

typedef struct ASDK_MediaCodec ASDK_MediaCodec;

SDL_AMediaCodec  *SDL_AMediaCodecJava_createByCodecName(JNIEnv *env, const char *codec_name);
jobject           SDL_AMediaCodecJava_getObject(JNIEnv *env, const SDL_AMediaCodec *thiz);

#endif
