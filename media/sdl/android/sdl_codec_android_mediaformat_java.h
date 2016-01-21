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

#ifndef SDL_ANDROID__ANDROID_CODEC_ANDROID_MEDIAFORMAT_JAVA_H
#define SDL_ANDROID__ANDROID_CODEC_ANDROID_MEDIAFORMAT_JAVA_H

#include "sdl_codec_android_mediaformat.h"

SDL_AMediaFormat *SDL_AMediaFormatJava_init(JNIEnv *env, jobject android_format);
SDL_AMediaFormat *SDL_AMediaFormatJava_createVideoFormat(JNIEnv *env, const char *mime, int width, int height);
jobject           SDL_AMediaFormatJava_getObject(JNIEnv *env, const SDL_AMediaFormat *thiz);

#endif

