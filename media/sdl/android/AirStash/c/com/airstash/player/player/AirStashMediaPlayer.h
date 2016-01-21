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

#ifndef AIRSTASH__com_airstash_player_player_AirStashMediaPlayer__H
#define AIRSTASH__com_airstash_player_player_AirStashMediaPlayer__H

#include "sdl/android/airstash/internal/internal.h"

jlong AirStashC_AirStashMediaPlayer__mNativeMediaPlayer__get(JNIEnv *env, jobject thiz);
jlong AirStashC_AirStashMediaPlayer__mNativeMediaPlayer__get__catchAll(JNIEnv *env, jobject thiz);
void AirStashC_AirStashMediaPlayer__mNativeMediaPlayer__set(JNIEnv *env, jobject thiz, jlong value);
void AirStashC_AirStashMediaPlayer__mNativeMediaPlayer__set__catchAll(JNIEnv *env, jobject thiz, jlong value);
jlong AirStashC_AirStashMediaPlayer__mNativeMediaDataSource__get(JNIEnv *env, jobject thiz);
jlong AirStashC_AirStashMediaPlayer__mNativeMediaDataSource__get__catchAll(JNIEnv *env, jobject thiz);
void AirStashC_AirStashMediaPlayer__mNativeMediaDataSource__set(JNIEnv *env, jobject thiz, jlong value);
void AirStashC_AirStashMediaPlayer__mNativeMediaDataSource__set__catchAll(JNIEnv *env, jobject thiz, jlong value);
void AirStashC_AirStashMediaPlayer__postEventFromNative(JNIEnv *env, jobject weakThiz, jint what, jint arg1, jint arg2, jobject obj);
void AirStashC_AirStashMediaPlayer__postEventFromNative__catchAll(JNIEnv *env, jobject weakThiz, jint what, jint arg1, jint arg2, jobject obj);
jstring AirStashC_AirStashMediaPlayer__onSelectCodec(JNIEnv *env, jobject weakThiz, jstring mimeType, jint profile, jint level);
jstring AirStashC_AirStashMediaPlayer__onSelectCodec__catchAll(JNIEnv *env, jobject weakThiz, jstring mimeType, jint profile, jint level);
jstring AirStashC_AirStashMediaPlayer__onSelectCodec__asGlobalRef__catchAll(JNIEnv *env, jobject weakThiz, jstring mimeType, jint profile, jint level);
const char *AirStashC_AirStashMediaPlayer__onSelectCodec__asCBuffer(JNIEnv *env, jobject weakThiz, jstring mimeType, jint profile, jint level, char *out_buf, int out_len);
const char *AirStashC_AirStashMediaPlayer__onSelectCodec__asCBuffer__catchAll(JNIEnv *env, jobject weakThiz, jstring mimeType, jint profile, jint level, char *out_buf, int out_len);
jstring AirStashC_AirStashMediaPlayer__onSelectCodec__withCString(JNIEnv *env, jobject weakThiz, const char *mimeType_cstr__, jint profile, jint level);
jstring AirStashC_AirStashMediaPlayer__onSelectCodec__withCString__catchAll(JNIEnv *env, jobject weakThiz, const char *mimeType_cstr__, jint profile, jint level);
jstring AirStashC_AirStashMediaPlayer__onSelectCodec__withCString__asGlobalRef__catchAll(JNIEnv *env, jobject weakThiz, const char *mimeType_cstr__, jint profile, jint level);
const char *AirStashC_AirStashMediaPlayer__onSelectCodec__withCString__asCBuffer(JNIEnv *env, jobject weakThiz, const char *mimeType_cstr__, jint profile, jint level, char *out_buf, int out_len);
const char *AirStashC_AirStashMediaPlayer__onSelectCodec__withCString__asCBuffer__catchAll(JNIEnv *env, jobject weakThiz, const char *mimeType_cstr__, jint profile, jint level, char *out_buf, int out_len);
jboolean AirStashC_AirStashMediaPlayer__onNativeInvoke(JNIEnv *env, jobject weakThiz, jint what, jobject args);
jboolean AirStashC_AirStashMediaPlayer__onNativeInvoke__catchAll(JNIEnv *env, jobject weakThiz, jint what, jobject args);
int AirStash_loadClass__AirStashC_AirStashMediaPlayer(JNIEnv *env);

#define AIRSTASH_HAVE__AirStashC_AirStashMediaPlayer

#endif//AIRSTASH__com_airstash_player_player_AirStashMediaPlayer__H
