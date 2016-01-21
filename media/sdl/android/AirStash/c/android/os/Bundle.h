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

#ifndef AIRSTASH__android_os_Bundle__H
#define AIRSTASH__android_os_Bundle__H

#include "sdl/android/airstash/internal/internal.h"

jobject AirStashC_Bundle__Bundle(JNIEnv *env);
jobject AirStashC_Bundle__Bundle__catchAll(JNIEnv *env);
jobject AirStashC_Bundle__Bundle__asGlobalRef__catchAll(JNIEnv *env);
jint AirStashC_Bundle__getInt(JNIEnv *env, jobject thiz, jstring key, jint defaultValue);
jint AirStashC_Bundle__getInt__catchAll(JNIEnv *env, jobject thiz, jstring key, jint defaultValue);
jint AirStashC_Bundle__getInt__withCString(JNIEnv *env, jobject thiz, const char *key_cstr__, jint defaultValue);
jint AirStashC_Bundle__getInt__withCString__catchAll(JNIEnv *env, jobject thiz, const char *key_cstr__, jint defaultValue);
void AirStashC_Bundle__putInt(JNIEnv *env, jobject thiz, jstring key, jint value);
void AirStashC_Bundle__putInt__catchAll(JNIEnv *env, jobject thiz, jstring key, jint value);
void AirStashC_Bundle__putInt__withCString(JNIEnv *env, jobject thiz, const char *key_cstr__, jint value);
void AirStashC_Bundle__putInt__withCString__catchAll(JNIEnv *env, jobject thiz, const char *key_cstr__, jint value);
jstring AirStashC_Bundle__getString(JNIEnv *env, jobject thiz, jstring key);
jstring AirStashC_Bundle__getString__catchAll(JNIEnv *env, jobject thiz, jstring key);
jstring AirStashC_Bundle__getString__asGlobalRef__catchAll(JNIEnv *env, jobject thiz, jstring key);
const char *AirStashC_Bundle__getString__asCBuffer(JNIEnv *env, jobject thiz, jstring key, char *out_buf, int out_len);
const char *AirStashC_Bundle__getString__asCBuffer__catchAll(JNIEnv *env, jobject thiz, jstring key, char *out_buf, int out_len);
jstring AirStashC_Bundle__getString__withCString(JNIEnv *env, jobject thiz, const char *key_cstr__);
jstring AirStashC_Bundle__getString__withCString__catchAll(JNIEnv *env, jobject thiz, const char *key_cstr__);
jstring AirStashC_Bundle__getString__withCString__asGlobalRef__catchAll(JNIEnv *env, jobject thiz, const char *key_cstr__);
const char *AirStashC_Bundle__getString__withCString__asCBuffer(JNIEnv *env, jobject thiz, const char *key_cstr__, char *out_buf, int out_len);
const char *AirStashC_Bundle__getString__withCString__asCBuffer__catchAll(JNIEnv *env, jobject thiz, const char *key_cstr__, char *out_buf, int out_len);
void AirStashC_Bundle__putString(JNIEnv *env, jobject thiz, jstring key, jstring value);
void AirStashC_Bundle__putString__catchAll(JNIEnv *env, jobject thiz, jstring key, jstring value);
void AirStashC_Bundle__putString__withCString(JNIEnv *env, jobject thiz, const char *key_cstr__, const char *value_cstr__);
void AirStashC_Bundle__putString__withCString__catchAll(JNIEnv *env, jobject thiz, const char *key_cstr__, const char *value_cstr__);
void AirStashC_Bundle__putParcelableArrayList(JNIEnv *env, jobject thiz, jstring key, jobject value);
void AirStashC_Bundle__putParcelableArrayList__catchAll(JNIEnv *env, jobject thiz, jstring key, jobject value);
void AirStashC_Bundle__putParcelableArrayList__withCString(JNIEnv *env, jobject thiz, const char *key_cstr__, jobject value);
void AirStashC_Bundle__putParcelableArrayList__withCString__catchAll(JNIEnv *env, jobject thiz, const char *key_cstr__, jobject value);
int AirStash_loadClass__AirStashC_Bundle(JNIEnv *env);

#define AIRSTASH_HAVE__AirStashC_Bundle

#endif//AIRSTASH__android_os_Bundle__H
