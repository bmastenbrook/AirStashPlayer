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

#ifndef AIRSTASH_INTERNAL_H
#define AIRSTASH_INTERNAL_H

#include <string.h>
#include <stdbool.h>
#include <jni.h>
#include "sdl/sdl_log.h"

#define AIRSTASH_FUNC_FAIL_TRACE()               do {ALOGE("%s: failed\n", __func__);} while (0)
#define AIRSTASH_FUNC_FAIL_TRACE1(x__)           do {ALOGE("%s: failed: %s\n", __func__, x__);} while (0)
#define AIRSTASH_FUNC_FAIL_TRACE2(x1__, x2__)    do {ALOGE("%s: failed: %s %s\n", __func__, x1__, x2__);} while (0)

#ifndef AIRSTASH_UNUSED
#define AIRSTASH_UNUSED(x) x __attribute__((unused))
#endif

/********************
 * Exception Handle
 ********************/

bool AirStash_ExceptionCheck__throwAny(JNIEnv *env);
bool AirStash_ExceptionCheck__catchAll(JNIEnv *env);
int  AirStash_ThrowExceptionOfClass(JNIEnv* env, jclass clazz, const char* msg);
int  AirStash_ThrowException(JNIEnv* env, const char* class_sign, const char* msg);
int  AirStash_ThrowIllegalStateException(JNIEnv *env, const char* msg);

/********************
 * References
 ********************/
jclass AirStash_NewGlobalRef__catchAll(JNIEnv *env, jobject obj);
void   AirStash_DeleteLocalRef(JNIEnv *env, jobject obj);
void   AirStash_DeleteLocalRef__p(JNIEnv *env, jobject *obj);
void   AirStash_DeleteGlobalRef(JNIEnv *env, jobject obj);
void   AirStash_DeleteGlobalRef__p(JNIEnv *env, jobject *obj);

void   AirStash_ReleaseStringUTFChars(JNIEnv *env, jstring str, const char *c_str);
void   AirStash_ReleaseStringUTFChars__p(JNIEnv *env, jstring str, const char **c_str);

/********************
 * Class Load
 ********************/

int    AirStash_LoadAll__catchAll(JNIEnv *env);
jclass AirStash_FindClass__catchAll(JNIEnv *env, const char *class_sign);
jclass AirStash_FindClass__asGlobalRef__catchAll(JNIEnv *env, const char *class_sign);

jmethodID AirStash_GetMethodID__catchAll(JNIEnv *env, jclass clazz, const char *method_name, const char *method_sign);
jmethodID AirStash_GetStaticMethodID__catchAll(JNIEnv *env, jclass clazz, const char *method_name, const char *method_sign);

jfieldID AirStash_GetFieldID__catchAll(JNIEnv *env, jclass clazz, const char *field_name, const char *method_sign);
jfieldID AirStash_GetStaticFieldID__catchAll(JNIEnv *env, jclass clazz, const char *field_name, const char *method_sign);

/********************
 * Misc Functions
 ********************/

jbyteArray AirStash_NewByteArray__catchAll(JNIEnv *env, jsize capacity);
jbyteArray AirStash_NewByteArray__asGlobalRef__catchAll(JNIEnv *env, jsize capacity);

int AirStash_GetSystemAndroidApiLevel(JNIEnv *env);

#endif//AIRSTASH_INTERNAL_H
