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

#include "MediaFormat.h"

typedef struct AirStashC_MediaFormat {
    jclass id;

    jmethodID constructor_MediaFormat;
    jmethodID method_createVideoFormat;
    jmethodID method_getInteger;
    jmethodID method_setInteger;
    jmethodID method_setByteBuffer;
} AirStashC_MediaFormat;
static AirStashC_MediaFormat class_AirStashC_MediaFormat;

jobject AirStashC_MediaFormat__MediaFormat(JNIEnv *env)
{
    return (*env)->NewObject(env, class_AirStashC_MediaFormat.id, class_AirStashC_MediaFormat.constructor_MediaFormat);
}

jobject AirStashC_MediaFormat__MediaFormat__catchAll(JNIEnv *env)
{
    jobject ret_object = AirStashC_MediaFormat__MediaFormat(env);
    if (AirStash_ExceptionCheck__catchAll(env) || !ret_object) {
        return NULL;
    }

    return ret_object;
}

jobject AirStashC_MediaFormat__MediaFormat__asGlobalRef__catchAll(JNIEnv *env)
{
    jobject ret_object   = NULL;
    jobject local_object = AirStashC_MediaFormat__MediaFormat__catchAll(env);
    if (AirStash_ExceptionCheck__catchAll(env) || !local_object) {
        ret_object = NULL;
        goto fail;
    }

    ret_object = AirStash_NewGlobalRef__catchAll(env, local_object);
    if (!ret_object) {
        ret_object = NULL;
        goto fail;
    }

fail:
    AirStash_DeleteLocalRef__p(env, &local_object);
    return ret_object;
}

jobject AirStashC_MediaFormat__createVideoFormat(JNIEnv *env, jstring mime, jint width, jint height)
{
    return (*env)->CallStaticObjectMethod(env, class_AirStashC_MediaFormat.id, class_AirStashC_MediaFormat.method_createVideoFormat, mime, width, height);
}

jobject AirStashC_MediaFormat__createVideoFormat__catchAll(JNIEnv *env, jstring mime, jint width, jint height)
{
    jobject ret_object = AirStashC_MediaFormat__createVideoFormat(env, mime, width, height);
    if (AirStash_ExceptionCheck__catchAll(env) || !ret_object) {
        return NULL;
    }

    return ret_object;
}

jobject AirStashC_MediaFormat__createVideoFormat__asGlobalRef__catchAll(JNIEnv *env, jstring mime, jint width, jint height)
{
    jobject ret_object   = NULL;
    jobject local_object = AirStashC_MediaFormat__createVideoFormat__catchAll(env, mime, width, height);
    if (AirStash_ExceptionCheck__catchAll(env) || !local_object) {
        ret_object = NULL;
        goto fail;
    }

    ret_object = AirStash_NewGlobalRef__catchAll(env, local_object);
    if (!ret_object) {
        ret_object = NULL;
        goto fail;
    }

fail:
    AirStash_DeleteLocalRef__p(env, &local_object);
    return ret_object;
}

jobject AirStashC_MediaFormat__createVideoFormat__withCString(JNIEnv *env, const char *mime_cstr__, jint width, jint height)
{
    jobject ret_object = NULL;
    jstring mime = NULL;

    mime = (*env)->NewStringUTF(env, mime_cstr__);
    if (AirStash_ExceptionCheck__throwAny(env) || !mime)
        goto fail;

    ret_object = AirStashC_MediaFormat__createVideoFormat(env, mime, width, height);
    if (AirStash_ExceptionCheck__throwAny(env) || !ret_object) {
        ret_object = NULL;
        goto fail;
    }

fail:
    AirStash_DeleteLocalRef__p(env, &mime);
    return ret_object;
}

jobject AirStashC_MediaFormat__createVideoFormat__withCString__catchAll(JNIEnv *env, const char *mime_cstr__, jint width, jint height)
{
    jobject ret_object = NULL;
    jstring mime = NULL;

    mime = (*env)->NewStringUTF(env, mime_cstr__);
    if (AirStash_ExceptionCheck__catchAll(env) || !mime)
        goto fail;

    ret_object = AirStashC_MediaFormat__createVideoFormat__catchAll(env, mime, width, height);
    if (AirStash_ExceptionCheck__catchAll(env) || !ret_object) {
        ret_object = NULL;
        goto fail;
    }

fail:
    AirStash_DeleteLocalRef__p(env, &mime);
    return ret_object;
}

jobject AirStashC_MediaFormat__createVideoFormat__withCString__asGlobalRef__catchAll(JNIEnv *env, const char *mime_cstr__, jint width, jint height)
{
    jobject ret_object   = NULL;
    jobject local_object = AirStashC_MediaFormat__createVideoFormat__withCString__catchAll(env, mime_cstr__, width, height);
    if (AirStash_ExceptionCheck__catchAll(env) || !local_object) {
        ret_object = NULL;
        goto fail;
    }

    ret_object = AirStash_NewGlobalRef__catchAll(env, local_object);
    if (!ret_object) {
        ret_object = NULL;
        goto fail;
    }

fail:
    AirStash_DeleteLocalRef__p(env, &local_object);
    return ret_object;
}

jint AirStashC_MediaFormat__getInteger(JNIEnv *env, jobject thiz, jstring name)
{
    return (*env)->CallIntMethod(env, thiz, class_AirStashC_MediaFormat.method_getInteger, name);
}

jint AirStashC_MediaFormat__getInteger__catchAll(JNIEnv *env, jobject thiz, jstring name)
{
    jint ret_value = AirStashC_MediaFormat__getInteger(env, thiz, name);
    if (AirStash_ExceptionCheck__catchAll(env)) {
        return 0;
    }

    return ret_value;
}

jint AirStashC_MediaFormat__getInteger__withCString(JNIEnv *env, jobject thiz, const char *name_cstr__)
{
    jint ret_value = 0;
    jstring name = NULL;

    name = (*env)->NewStringUTF(env, name_cstr__);
    if (AirStash_ExceptionCheck__throwAny(env) || !name)
        goto fail;

    ret_value = AirStashC_MediaFormat__getInteger(env, thiz, name);
    if (AirStash_ExceptionCheck__throwAny(env)) {
        ret_value = 0;
        goto fail;
    }

fail:
    AirStash_DeleteLocalRef__p(env, &name);
    return ret_value;
}

jint AirStashC_MediaFormat__getInteger__withCString__catchAll(JNIEnv *env, jobject thiz, const char *name_cstr__)
{
    jint ret_value = 0;
    jstring name = NULL;

    name = (*env)->NewStringUTF(env, name_cstr__);
    if (AirStash_ExceptionCheck__catchAll(env) || !name)
        goto fail;

    ret_value = AirStashC_MediaFormat__getInteger__catchAll(env, thiz, name);
    if (AirStash_ExceptionCheck__catchAll(env)) {
        ret_value = 0;
        goto fail;
    }

fail:
    AirStash_DeleteLocalRef__p(env, &name);
    return ret_value;
}

void AirStashC_MediaFormat__setInteger(JNIEnv *env, jobject thiz, jstring name, jint value)
{
    (*env)->CallVoidMethod(env, thiz, class_AirStashC_MediaFormat.method_setInteger, name, value);
}

void AirStashC_MediaFormat__setInteger__catchAll(JNIEnv *env, jobject thiz, jstring name, jint value)
{
    AirStashC_MediaFormat__setInteger(env, thiz, name, value);
    AirStash_ExceptionCheck__catchAll(env);
}

void AirStashC_MediaFormat__setInteger__withCString(JNIEnv *env, jobject thiz, const char *name_cstr__, jint value)
{
    jstring name = NULL;

    name = (*env)->NewStringUTF(env, name_cstr__);
    if (AirStash_ExceptionCheck__throwAny(env) || !name)
        goto fail;

    AirStashC_MediaFormat__setInteger(env, thiz, name, value);

fail:
    AirStash_DeleteLocalRef__p(env, &name);
}

void AirStashC_MediaFormat__setInteger__withCString__catchAll(JNIEnv *env, jobject thiz, const char *name_cstr__, jint value)
{
    jstring name = NULL;

    name = (*env)->NewStringUTF(env, name_cstr__);
    if (AirStash_ExceptionCheck__catchAll(env) || !name)
        goto fail;

    AirStashC_MediaFormat__setInteger__catchAll(env, thiz, name, value);

fail:
    AirStash_DeleteLocalRef__p(env, &name);
}

void AirStashC_MediaFormat__setByteBuffer(JNIEnv *env, jobject thiz, jstring name, jobject bytes)
{
    (*env)->CallVoidMethod(env, thiz, class_AirStashC_MediaFormat.method_setByteBuffer, name, bytes);
}

void AirStashC_MediaFormat__setByteBuffer__catchAll(JNIEnv *env, jobject thiz, jstring name, jobject bytes)
{
    AirStashC_MediaFormat__setByteBuffer(env, thiz, name, bytes);
    AirStash_ExceptionCheck__catchAll(env);
}

void AirStashC_MediaFormat__setByteBuffer__withCString(JNIEnv *env, jobject thiz, const char *name_cstr__, jobject bytes)
{
    jstring name = NULL;

    name = (*env)->NewStringUTF(env, name_cstr__);
    if (AirStash_ExceptionCheck__throwAny(env) || !name)
        goto fail;

    AirStashC_MediaFormat__setByteBuffer(env, thiz, name, bytes);

fail:
    AirStash_DeleteLocalRef__p(env, &name);
}

void AirStashC_MediaFormat__setByteBuffer__withCString__catchAll(JNIEnv *env, jobject thiz, const char *name_cstr__, jobject bytes)
{
    jstring name = NULL;

    name = (*env)->NewStringUTF(env, name_cstr__);
    if (AirStash_ExceptionCheck__catchAll(env) || !name)
        goto fail;

    AirStashC_MediaFormat__setByteBuffer__catchAll(env, thiz, name, bytes);

fail:
    AirStash_DeleteLocalRef__p(env, &name);
}

int AirStash_loadClass__AirStashC_MediaFormat(JNIEnv *env)
{
    int         ret                   = -1;
    const char *AIRSTASH_UNUSED(name)      = NULL;
    const char *AIRSTASH_UNUSED(sign)      = NULL;
    jclass      AIRSTASH_UNUSED(class_id)  = NULL;
    int         AIRSTASH_UNUSED(api_level) = 0;

    api_level = AirStash_GetSystemAndroidApiLevel(env);

    if (api_level < 16) {
        ALOGW("AirStashLoader: Ignore: '%s' need API %d\n", "android.media.MediaFormat", api_level);
        goto ignore;
    }

    sign = "android/media/MediaFormat";
    class_AirStashC_MediaFormat.id = AirStash_FindClass__asGlobalRef__catchAll(env, sign);
    if (class_AirStashC_MediaFormat.id == NULL)
        goto fail;

    class_id = class_AirStashC_MediaFormat.id;
    name     = "<init>";
    sign     = "()V";
    class_AirStashC_MediaFormat.constructor_MediaFormat = AirStash_GetMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_MediaFormat.constructor_MediaFormat == NULL)
        goto fail;

    class_id = class_AirStashC_MediaFormat.id;
    name     = "createVideoFormat";
    sign     = "(Ljava/lang/String;II)Landroid/media/MediaFormat;";
    class_AirStashC_MediaFormat.method_createVideoFormat = AirStash_GetStaticMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_MediaFormat.method_createVideoFormat == NULL)
        goto fail;

    class_id = class_AirStashC_MediaFormat.id;
    name     = "getInteger";
    sign     = "(Ljava/lang/String;)I";
    class_AirStashC_MediaFormat.method_getInteger = AirStash_GetMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_MediaFormat.method_getInteger == NULL)
        goto fail;

    class_id = class_AirStashC_MediaFormat.id;
    name     = "setInteger";
    sign     = "(Ljava/lang/String;I)V";
    class_AirStashC_MediaFormat.method_setInteger = AirStash_GetMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_MediaFormat.method_setInteger == NULL)
        goto fail;

    class_id = class_AirStashC_MediaFormat.id;
    name     = "setByteBuffer";
    sign     = "(Ljava/lang/String;Ljava/nio/ByteBuffer;)V";
    class_AirStashC_MediaFormat.method_setByteBuffer = AirStash_GetMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_MediaFormat.method_setByteBuffer == NULL)
        goto fail;

    ALOGD("AirtashLoader: OK: '%s' loaded\n", "android.media.MediaFormat");
ignore:
    ret = 0;
fail:
    return ret;
}
