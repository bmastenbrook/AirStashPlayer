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

#include "internal.h"
#include "../c/android/os/Build.h"

/********************
 * Exception Handle
 ********************/

bool AirStash_ExceptionCheck__throwAny(JNIEnv *env)
{
    if ((*env)->ExceptionCheck(env)) {
        (*env)->ExceptionDescribe(env);
        return true;
    }

    return false;
}

bool AirStash_ExceptionCheck__catchAll(JNIEnv *env)
{
    if ((*env)->ExceptionCheck(env)) {
        (*env)->ExceptionDescribe(env);
        (*env)->ExceptionClear(env);
        return true;
    }

    return false;
}

int AirStash_ThrowExceptionOfClass(JNIEnv* env, jclass clazz, const char* msg)
{
    if ((*env)->ThrowNew(env, clazz, msg) != JNI_OK)
        ALOGE("%s: Failed: msg: '%s'\n", __func__, msg);

    return 0;
}

int AirStash_ThrowException(JNIEnv* env, const char* class_sign, const char* msg)
{
    int ret = -1;

    if (AirStash_ExceptionCheck__catchAll(env)) {
        ALOGE("pending exception throwed.\n");
    }

    jclass exceptionClass = AirStash_FindClass__catchAll(env, class_sign);
    if (exceptionClass == NULL) {
        AIRSTASH_FUNC_FAIL_TRACE();
        ret = -1;
        goto fail;
    }

    ret = AirStash_ThrowExceptionOfClass(env, exceptionClass, msg);
    if (ret) {
        AIRSTASH_FUNC_FAIL_TRACE();
        goto fail;
    }

    ret = 0;
fail:
    AirStash_DeleteLocalRef__p(env, exceptionClass);
    return ret;
}

int AirStash_ThrowIllegalStateException(JNIEnv *env, const char* msg)
{
    return AirStash_ThrowException(env, "java/lang/IllegalStateException", msg);
}

/********************
 * References
 ********************/

jclass AirStash_NewGlobalRef__catchAll(JNIEnv *env, jobject obj)
{
    jclass obj_global = (*env)->NewGlobalRef(env, obj);
    if (AirStash_ExceptionCheck__catchAll(env) || !(obj_global)) {
        AIRSTASH_FUNC_FAIL_TRACE();
        goto fail;
    }

fail:
    return obj_global;
}

void AirStash_DeleteLocalRef(JNIEnv *env, jobject obj)
{
    if (!obj)
        return;
    (*env)->DeleteLocalRef(env, obj);
}

void AirStash_DeleteLocalRef__p(JNIEnv *env, jobject *obj)
{
    if (!obj)
        return;
    AirStash_DeleteLocalRef(env, *obj);
    *obj = NULL;
}

void AirStash_DeleteGlobalRef(JNIEnv *env, jobject obj)
{
    if (!obj)
        return;
    (*env)->DeleteGlobalRef(env, obj);
}

void AirStash_DeleteGlobalRef__p(JNIEnv *env, jobject *obj)
{
    if (!obj)
        return;
    AirStash_DeleteGlobalRef(env, *obj);
    *obj = NULL;
}

void AirStash_ReleaseStringUTFChars(JNIEnv *env, jstring str, const char *c_str)
{
    if (!str || !c_str)
        return;
    (*env)->ReleaseStringUTFChars(env, str, c_str);
}

void AirStash_ReleaseStringUTFChars__p(JNIEnv *env, jstring str, const char **c_str)
{
    if (!str || !c_str)
        return;
    AirStash_ReleaseStringUTFChars(env, str, *c_str);
    *c_str = NULL;
}

/********************
 * Class Load
 ********************/

jclass AirStash_FindClass__catchAll(JNIEnv *env, const char *class_sign)
{
    jclass clazz = (*env)->FindClass(env, class_sign);
    if (AirStash_ExceptionCheck__catchAll(env) || !(clazz)) {
        AIRSTASH_FUNC_FAIL_TRACE();
        clazz = NULL;
        goto fail;
    }

fail:
    return clazz;
}

jclass AirStash_FindClass__asGlobalRef__catchAll(JNIEnv *env, const char *class_sign)
{
    jclass clazz_global = NULL;
    jclass clazz = AirStash_FindClass__catchAll(env, class_sign);
    if (!clazz) {
        AIRSTASH_FUNC_FAIL_TRACE1(class_sign);
        goto fail;
    }

    clazz_global = AirStash_NewGlobalRef__catchAll(env, clazz);
    if (!clazz_global) {
        AIRSTASH_FUNC_FAIL_TRACE1(class_sign);
        goto fail;
    }

fail:
    AirStash_DeleteLocalRef__p(env, &clazz);
    return clazz_global;
}

jmethodID AirStash_GetMethodID__catchAll(JNIEnv *env, jclass clazz, const char *method_name, const char *method_sign)
{
    jmethodID method_id = (*env)->GetMethodID(env, clazz, method_name, method_sign);
    if (AirStash_ExceptionCheck__catchAll(env) || !method_id) {
        AIRSTASH_FUNC_FAIL_TRACE2(method_name, method_sign);
        method_id = NULL;
        goto fail;
    }

fail:
    return method_id;
}

jmethodID AirStash_GetStaticMethodID__catchAll(JNIEnv *env, jclass clazz, const char *method_name, const char *method_sign)
{
    jmethodID method_id = (*env)->GetStaticMethodID(env, clazz, method_name, method_sign);
    if (AirStash_ExceptionCheck__catchAll(env) || !method_id) {
        AIRSTASH_FUNC_FAIL_TRACE2(method_name, method_sign);
        method_id = NULL;
        goto fail;
    }

fail:
    return method_id;
}

jfieldID AirStash_GetFieldID__catchAll(JNIEnv *env, jclass clazz, const char *field_name, const char *field_sign)
{
    jfieldID field_id = (*env)->GetFieldID(env, clazz, field_name, field_sign);
    if (AirStash_ExceptionCheck__catchAll(env) || !field_id) {
        AIRSTASH_FUNC_FAIL_TRACE2(field_name, field_sign);
        field_id = NULL;
        goto fail;
    }

fail:
    return field_id;
}

jfieldID AirStash_GetStaticFieldID__catchAll(JNIEnv *env, jclass clazz, const char *field_name, const char *field_sign)
{
    jfieldID field_id = (*env)->GetStaticFieldID(env, clazz, field_name, field_sign);
    if (AirStash_ExceptionCheck__catchAll(env) || !field_id) {
        AIRSTASH_FUNC_FAIL_TRACE2(field_name, field_sign);
        field_id = NULL;
        goto fail;
    }

fail:
    return field_id;
}

/********************
 * Misc Functions
 ********************/

jbyteArray AirStash_NewByteArray__catchAll(JNIEnv *env, jsize capacity)
{
    jbyteArray local = (*env)->NewByteArray(env, capacity);
    if (AirStash_ExceptionCheck__catchAll(env) || !local)
        return NULL;

    return local;
}

jbyteArray AirStash_NewByteArray__asGlobalRef__catchAll(JNIEnv *env, jsize capacity)
{
    jbyteArray local = (*env)->NewByteArray(env, capacity);
    if (AirStash_ExceptionCheck__catchAll(env) || !local)
        return NULL;

    jbyteArray global = (*env)->NewGlobalRef(env, local);
    AirStash_DeleteLocalRef__p(env, &local);
    return global;
}

int AirStash_GetSystemAndroidApiLevel(JNIEnv *env)
{
    static int SDK_INT = 0;
    if (SDK_INT > 0)
        return SDK_INT;

    SDK_INT = AirStashC_android_os_Build__VERSION__SDK_INT__get__catchAll(env);
    ALOGI("API-Level: %d\n", SDK_INT);
    return SDK_INT;
}
