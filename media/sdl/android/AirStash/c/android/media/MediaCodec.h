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

#ifndef AirStash__android_media_MediaCodec__H
#define AirStash__android_media_MediaCodec__H

#include "sdl/android/airstash/internal/internal.h"

jint AirStashC_MediaCodec__BufferInfo__flags__get(JNIEnv *env, jobject thiz);
jint AirStashC_MediaCodec__BufferInfo__flags__get__catchAll(JNIEnv *env, jobject thiz);
void AirStashC_MediaCodec__BufferInfo__flags__set(JNIEnv *env, jobject thiz, jint value);
void AirStashC_MediaCodec__BufferInfo__flags__set__catchAll(JNIEnv *env, jobject thiz, jint value);
jint AirStashC_MediaCodec__BufferInfo__offset__get(JNIEnv *env, jobject thiz);
jint AirStashC_MediaCodec__BufferInfo__offset__get__catchAll(JNIEnv *env, jobject thiz);
void AirStashC_MediaCodec__BufferInfo__offset__set(JNIEnv *env, jobject thiz, jint value);
void AirStashC_MediaCodec__BufferInfo__offset__set__catchAll(JNIEnv *env, jobject thiz, jint value);
jlong AirStashC_MediaCodec__BufferInfo__presentationTimeUs__get(JNIEnv *env, jobject thiz);
jlong AirStashC_MediaCodec__BufferInfo__presentationTimeUs__get__catchAll(JNIEnv *env, jobject thiz);
void AirStashC_MediaCodec__BufferInfo__presentationTimeUs__set(JNIEnv *env, jobject thiz, jlong value);
void AirStashC_MediaCodec__BufferInfo__presentationTimeUs__set__catchAll(JNIEnv *env, jobject thiz, jlong value);
jint AirStashC_MediaCodec__BufferInfo__size__get(JNIEnv *env, jobject thiz);
jint AirStashC_MediaCodec__BufferInfo__size__get__catchAll(JNIEnv *env, jobject thiz);
void AirStashC_MediaCodec__BufferInfo__size__set(JNIEnv *env, jobject thiz, jint value);
void AirStashC_MediaCodec__BufferInfo__size__set__catchAll(JNIEnv *env, jobject thiz, jint value);
jobject AirStashC_MediaCodec__BufferInfo__BufferInfo(JNIEnv *env);
jobject AirStashC_MediaCodec__BufferInfo__BufferInfo__catchAll(JNIEnv *env);
jobject AirStashC_MediaCodec__BufferInfo__BufferInfo__asGlobalRef__catchAll(JNIEnv *env);
jobject AirStashC_MediaCodec__createByCodecName(JNIEnv *env, jstring name);
jobject AirStashC_MediaCodec__createByCodecName__catchAll(JNIEnv *env, jstring name);
jobject AirStashC_MediaCodec__createByCodecName__asGlobalRef__catchAll(JNIEnv *env, jstring name);
jobject AirStashC_MediaCodec__createByCodecName__withCString(JNIEnv *env, const char *name_cstr__);
jobject AirStashC_MediaCodec__createByCodecName__withCString__catchAll(JNIEnv *env, const char *name_cstr__);
jobject AirStashC_MediaCodec__createByCodecName__withCString__asGlobalRef__catchAll(JNIEnv *env, const char *name_cstr__);
void AirStashC_MediaCodec__configure(JNIEnv *env, jobject thiz, jobject format, jobject surface, jobject crypto, jint flags);
void AirStashC_MediaCodec__configure__catchAll(JNIEnv *env, jobject thiz, jobject format, jobject surface, jobject crypto, jint flags);
jobject AirStashC_MediaCodec__getOutputFormat(JNIEnv *env, jobject thiz);
jobject AirStashC_MediaCodec__getOutputFormat__catchAll(JNIEnv *env, jobject thiz);
jobject AirStashC_MediaCodec__getOutputFormat__asGlobalRef__catchAll(JNIEnv *env, jobject thiz);
jobjectArray AirStashC_MediaCodec__getInputBuffers(JNIEnv *env, jobject thiz);
jobjectArray AirStashC_MediaCodec__getInputBuffers__catchAll(JNIEnv *env, jobject thiz);
jobjectArray AirStashC_MediaCodec__getInputBuffers__asGlobalRef__catchAll(JNIEnv *env, jobject thiz);
jint AirStashC_MediaCodec__dequeueInputBuffer(JNIEnv *env, jobject thiz, jlong timeoutUs);
jint AirStashC_MediaCodec__dequeueInputBuffer__catchAll(JNIEnv *env, jobject thiz, jlong timeoutUs);
void AirStashC_MediaCodec__queueInputBuffer(JNIEnv *env, jobject thiz, jint index, jint offset, jint size, jlong presentationTimeUs, jint flags);
void AirStashC_MediaCodec__queueInputBuffer__catchAll(JNIEnv *env, jobject thiz, jint index, jint offset, jint size, jlong presentationTimeUs, jint flags);
jint AirStashC_MediaCodec__dequeueOutputBuffer(JNIEnv *env, jobject thiz, jobject info, jlong timeoutUs);
jint AirStashC_MediaCodec__dequeueOutputBuffer__catchAll(JNIEnv *env, jobject thiz, jobject info, jlong timeoutUs);
void AirStashC_MediaCodec__releaseOutputBuffer(JNIEnv *env, jobject thiz, jint index, jboolean render);
void AirStashC_MediaCodec__releaseOutputBuffer__catchAll(JNIEnv *env, jobject thiz, jint index, jboolean render);
void AirStashC_MediaCodec__start(JNIEnv *env, jobject thiz);
void AirStashC_MediaCodec__start__catchAll(JNIEnv *env, jobject thiz);
void AirStashC_MediaCodec__stop(JNIEnv *env, jobject thiz);
void AirStashC_MediaCodec__stop__catchAll(JNIEnv *env, jobject thiz);
void AirStashC_MediaCodec__flush(JNIEnv *env, jobject thiz);
void AirStashC_MediaCodec__flush__catchAll(JNIEnv *env, jobject thiz);
void AirStashC_MediaCodec__release(JNIEnv *env, jobject thiz);
void AirStashC_MediaCodec__release__catchAll(JNIEnv *env, jobject thiz);
int AirStash_loadClass__AirStashC_MediaCodec(JNIEnv *env);

#define AirStash_HAVE__AirStashC_MediaCodec

#endif//AirStash__android_media_MediaCodec__H
