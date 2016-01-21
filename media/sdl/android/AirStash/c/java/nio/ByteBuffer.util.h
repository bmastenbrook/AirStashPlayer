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

#ifndef AIRSTASH__java_nio_ByteBuffer__UTIL__H
#define AIRSTASH__java_nio_ByteBuffer__UTIL__H

#include "sdl/android/AirStash/internal/internal.h"
#include "ByteBuffer.h"

void *AirStashC_java_nio_ByteBuffer__getDirectBufferAddress(JNIEnv *env, jobject thiz);
void *AirStashC_java_nio_ByteBuffer__getDirectBufferAddress__catchAll(JNIEnv *env, jobject thiz);
int   AirStashC_java_nio_ByteBuffer__assignData__catchAll(JNIEnv *env, jobject thiz, void* data, size_t size);

#ifdef AIRSTASH_HAVE__AirStashC_ByteBuffer
inline static void *AirStashC_ByteBuffer__getDirectBufferAddress(JNIEnv *env, jobject thiz) {return AirStashC_java_nio_ByteBuffer__getDirectBufferAddress(env, thiz);}
inline static void *AirStashC_ByteBuffer__getDirectBufferAddress__catchAll(JNIEnv *env, jobject thiz) {return AirStashC_java_nio_ByteBuffer__getDirectBufferAddress__catchAll(env, thiz);}
inline static int   AirStashC_ByteBuffer__assignData__catchAll(JNIEnv *env, jobject thiz, void* data, size_t size) {return AirStashC_java_nio_ByteBuffer__assignData__catchAll(env, thiz, data, size);}
#endif

#endif
