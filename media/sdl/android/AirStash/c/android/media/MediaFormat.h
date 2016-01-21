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

#ifndef AirStash__android_media_MediaFormat__H
#define AirStash__android_media_MediaFormat__H

#include "sdl/android/airstash/internal/internal.h"

jobject AirStashC_MediaFormat__MediaFormat(JNIEnv *env);
jobject AirStashC_MediaFormat__MediaFormat__catchAll(JNIEnv *env);
jobject AirStashC_MediaFormat__MediaFormat__asGlobalRef__catchAll(JNIEnv *env);
jobject AirStashC_MediaFormat__createVideoFormat(JNIEnv *env, jstring mime, jint width, jint height);
jobject AirStashC_MediaFormat__createVideoFormat__catchAll(JNIEnv *env, jstring mime, jint width, jint height);
jobject AirStashC_MediaFormat__createVideoFormat__asGlobalRef__catchAll(JNIEnv *env, jstring mime, jint width, jint height);
jobject AirStashC_MediaFormat__createVideoFormat__withCString(JNIEnv *env, const char *mime_cstr__, jint width, jint height);
jobject AirStashC_MediaFormat__createVideoFormat__withCString__catchAll(JNIEnv *env, const char *mime_cstr__, jint width, jint height);
jobject AirStashC_MediaFormat__createVideoFormat__withCString__asGlobalRef__catchAll(JNIEnv *env, const char *mime_cstr__, jint width, jint height);
jint AirStashC_MediaFormat__getInteger(JNIEnv *env, jobject thiz, jstring name);
jint AirStashC_MediaFormat__getInteger__catchAll(JNIEnv *env, jobject thiz, jstring name);
jint AirStashC_MediaFormat__getInteger__withCString(JNIEnv *env, jobject thiz, const char *name_cstr__);
jint AirStashC_MediaFormat__getInteger__withCString__catchAll(JNIEnv *env, jobject thiz, const char *name_cstr__);
void AirStashC_MediaFormat__setInteger(JNIEnv *env, jobject thiz, jstring name, jint value);
void AirStashC_MediaFormat__setInteger__catchAll(JNIEnv *env, jobject thiz, jstring name, jint value);
void AirStashC_MediaFormat__setInteger__withCString(JNIEnv *env, jobject thiz, const char *name_cstr__, jint value);
void AirStashC_MediaFormat__setInteger__withCString__catchAll(JNIEnv *env, jobject thiz, const char *name_cstr__, jint value);
void AirStashC_MediaFormat__setByteBuffer(JNIEnv *env, jobject thiz, jstring name, jobject bytes);
void AirStashC_MediaFormat__setByteBuffer__catchAll(JNIEnv *env, jobject thiz, jstring name, jobject bytes);
void AirStashC_MediaFormat__setByteBuffer__withCString(JNIEnv *env, jobject thiz, const char *name_cstr__, jobject bytes);
void AirStashC_MediaFormat__setByteBuffer__withCString__catchAll(JNIEnv *env, jobject thiz, const char *name_cstr__, jobject bytes);
int AirStash_loadClass__AirStashC_MediaFormat(JNIEnv *env);

#define AIRSTASH_HAVE__AIRSTASHC_MediaFormat

#endif//AIRSTASH__android_media_MediaFormat__H
