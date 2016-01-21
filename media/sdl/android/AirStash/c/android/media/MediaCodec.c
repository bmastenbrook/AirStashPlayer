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

#include "MediaCodec.h"

typedef struct AirStashC_MediaCodec__BufferInfo {
    jclass id;

    jfieldID field_flags;
    jfieldID field_offset;
    jfieldID field_presentationTimeUs;
    jfieldID field_size;
    jmethodID constructor_BufferInfo;
} AirStashC_MediaCodec__BufferInfo;
static AirStashC_MediaCodec__BufferInfo class_AirStashC_MediaCodec__BufferInfo;

typedef struct AirStashC_MediaCodec {
    jclass id;

    jmethodID method_createByCodecName;
    jmethodID method_configure;
    jmethodID method_getOutputFormat;
    jmethodID method_getInputBuffers;
    jmethodID method_dequeueInputBuffer;
    jmethodID method_queueInputBuffer;
    jmethodID method_dequeueOutputBuffer;
    jmethodID method_releaseOutputBuffer;
    jmethodID method_start;
    jmethodID method_stop;
    jmethodID method_flush;
    jmethodID method_release;
} AirStashC_MediaCodec;
static AirStashC_MediaCodec class_AirStashC_MediaCodec;

jint AirStashC_MediaCodec__BufferInfo__flags__get(JNIEnv *env, jobject thiz)
{
    return (*env)->GetIntField(env, thiz, class_AirStashC_MediaCodec__BufferInfo.field_flags);
}

jint AirStashC_MediaCodec__BufferInfo__flags__get__catchAll(JNIEnv *env, jobject thiz)
{
    jint ret_value = AirStashC_MediaCodec__BufferInfo__flags__get(env, thiz);
    if (AirStash_ExceptionCheck__catchAll(env)) {
        return 0;
    }

    return ret_value;
}

void AirStashC_MediaCodec__BufferInfo__flags__set(JNIEnv *env, jobject thiz, jint value)
{
    (*env)->SetIntField(env, thiz, class_AirStashC_MediaCodec__BufferInfo.field_flags, value);
}

void AirStashC_MediaCodec__BufferInfo__flags__set__catchAll(JNIEnv *env, jobject thiz, jint value)
{
    AirStashC_MediaCodec__BufferInfo__flags__set(env, thiz, value);
    AirStash_ExceptionCheck__catchAll(env);
}

jint AirStashC_MediaCodec__BufferInfo__offset__get(JNIEnv *env, jobject thiz)
{
    return (*env)->GetIntField(env, thiz, class_AirStashC_MediaCodec__BufferInfo.field_offset);
}

jint AirStashC_MediaCodec__BufferInfo__offset__get__catchAll(JNIEnv *env, jobject thiz)
{
    jint ret_value = AirStashC_MediaCodec__BufferInfo__offset__get(env, thiz);
    if (AirStash_ExceptionCheck__catchAll(env)) {
        return 0;
    }

    return ret_value;
}

void AirStashC_MediaCodec__BufferInfo__offset__set(JNIEnv *env, jobject thiz, jint value)
{
    (*env)->SetIntField(env, thiz, class_AirStashC_MediaCodec__BufferInfo.field_offset, value);
}

void AirStashC_MediaCodec__BufferInfo__offset__set__catchAll(JNIEnv *env, jobject thiz, jint value)
{
    AirStashC_MediaCodec__BufferInfo__offset__set(env, thiz, value);
    AirStash_ExceptionCheck__catchAll(env);
}

jlong AirStashC_MediaCodec__BufferInfo__presentationTimeUs__get(JNIEnv *env, jobject thiz)
{
    return (*env)->GetLongField(env, thiz, class_AirStashC_MediaCodec__BufferInfo.field_presentationTimeUs);
}

jlong AirStashC_MediaCodec__BufferInfo__presentationTimeUs__get__catchAll(JNIEnv *env, jobject thiz)
{
    jlong ret_value = AirStashC_MediaCodec__BufferInfo__presentationTimeUs__get(env, thiz);
    if (AirStash_ExceptionCheck__catchAll(env)) {
        return 0;
    }

    return ret_value;
}

void AirStashC_MediaCodec__BufferInfo__presentationTimeUs__set(JNIEnv *env, jobject thiz, jlong value)
{
    (*env)->SetLongField(env, thiz, class_AirStashC_MediaCodec__BufferInfo.field_presentationTimeUs, value);
}

void AirStashC_MediaCodec__BufferInfo__presentationTimeUs__set__catchAll(JNIEnv *env, jobject thiz, jlong value)
{
    AirStashC_MediaCodec__BufferInfo__presentationTimeUs__set(env, thiz, value);
    AirStash_ExceptionCheck__catchAll(env);
}

jint AirStashC_MediaCodec__BufferInfo__size__get(JNIEnv *env, jobject thiz)
{
    return (*env)->GetIntField(env, thiz, class_AirStashC_MediaCodec__BufferInfo.field_size);
}

jint AirStashC_MediaCodec__BufferInfo__size__get__catchAll(JNIEnv *env, jobject thiz)
{
    jint ret_value = AirStashC_MediaCodec__BufferInfo__size__get(env, thiz);
    if (AirStash_ExceptionCheck__catchAll(env)) {
        return 0;
    }

    return ret_value;
}

void AirStashC_MediaCodec__BufferInfo__size__set(JNIEnv *env, jobject thiz, jint value)
{
    (*env)->SetIntField(env, thiz, class_AirStashC_MediaCodec__BufferInfo.field_size, value);
}

void AirStashC_MediaCodec__BufferInfo__size__set__catchAll(JNIEnv *env, jobject thiz, jint value)
{
    AirStashC_MediaCodec__BufferInfo__size__set(env, thiz, value);
    AirStash_ExceptionCheck__catchAll(env);
}

jobject AirStashC_MediaCodec__BufferInfo__BufferInfo(JNIEnv *env)
{
    return (*env)->NewObject(env, class_AirStashC_MediaCodec__BufferInfo.id, class_AirStashC_MediaCodec__BufferInfo.constructor_BufferInfo);
}

jobject AirStashC_MediaCodec__BufferInfo__BufferInfo__catchAll(JNIEnv *env)
{
    jobject ret_object = AirStashC_MediaCodec__BufferInfo__BufferInfo(env);
    if (AirStash_ExceptionCheck__catchAll(env) || !ret_object) {
        return NULL;
    }

    return ret_object;
}

jobject AirStashC_MediaCodec__BufferInfo__BufferInfo__asGlobalRef__catchAll(JNIEnv *env)
{
    jobject ret_object   = NULL;
    jobject local_object = AirStashC_MediaCodec__BufferInfo__BufferInfo__catchAll(env);
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

int AirStash_loadClass__AirStashC_MediaCodec__BufferInfo(JNIEnv *env)
{
    int         ret                   = -1;
    const char *AIRSTASH_UNUSED(name)      = NULL;
    const char *AIRSTASH_UNUSED(sign)      = NULL;
    jclass      AIRSTASH_UNUSED(class_id)  = NULL;
    int         AIRSTASH_UNUSED(api_level) = 0;

    sign = "android/media/MediaCodec$BufferInfo";
    class_AirStashC_MediaCodec__BufferInfo.id = AirStash_FindClass__asGlobalRef__catchAll(env, sign);
    if (class_AirStashC_MediaCodec__BufferInfo.id == NULL)
        goto fail;

    class_id = class_AirStashC_MediaCodec__BufferInfo.id;
    name     = "flags";
    sign     = "I";
    class_AirStashC_MediaCodec__BufferInfo.field_flags = AirStash_GetFieldID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_MediaCodec__BufferInfo.field_flags == NULL)
        goto fail;

    class_id = class_AirStashC_MediaCodec__BufferInfo.id;
    name     = "offset";
    sign     = "I";
    class_AirStashC_MediaCodec__BufferInfo.field_offset = AirStash_GetFieldID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_MediaCodec__BufferInfo.field_offset == NULL)
        goto fail;

    class_id = class_AirStashC_MediaCodec__BufferInfo.id;
    name     = "presentationTimeUs";
    sign     = "J";
    class_AirStashC_MediaCodec__BufferInfo.field_presentationTimeUs = AirStash_GetFieldID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_MediaCodec__BufferInfo.field_presentationTimeUs == NULL)
        goto fail;

    class_id = class_AirStashC_MediaCodec__BufferInfo.id;
    name     = "size";
    sign     = "I";
    class_AirStashC_MediaCodec__BufferInfo.field_size = AirStash_GetFieldID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_MediaCodec__BufferInfo.field_size == NULL)
        goto fail;

    class_id = class_AirStashC_MediaCodec__BufferInfo.id;
    name     = "<init>";
    sign     = "()V";
    class_AirStashC_MediaCodec__BufferInfo.constructor_BufferInfo = AirStash_GetMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_MediaCodec__BufferInfo.constructor_BufferInfo == NULL)
        goto fail;

    ALOGD("AirStashLoader: OK: '%s' loaded\n", "android.media.MediaCodec$BufferInfo");
    ret = 0;
fail:
    return ret;
}

jobject AirStashC_MediaCodec__createByCodecName(JNIEnv *env, jstring name)
{
    return (*env)->CallStaticObjectMethod(env, class_AirStashC_MediaCodec.id, class_AirStashC_MediaCodec.method_createByCodecName, name);
}

jobject AirStashC_MediaCodec__createByCodecName__catchAll(JNIEnv *env, jstring name)
{
    jobject ret_object = AirStashC_MediaCodec__createByCodecName(env, name);
    if (AirStash_ExceptionCheck__catchAll(env) || !ret_object) {
        return NULL;
    }

    return ret_object;
}

jobject AirStashC_MediaCodec__createByCodecName__asGlobalRef__catchAll(JNIEnv *env, jstring name)
{
    jobject ret_object   = NULL;
    jobject local_object = AirStashC_MediaCodec__createByCodecName__catchAll(env, name);
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

jobject AirStashC_MediaCodec__createByCodecName__withCString(JNIEnv *env, const char *name_cstr__)
{
    jobject ret_object = NULL;
    jstring name = NULL;

    name = (*env)->NewStringUTF(env, name_cstr__);
    if (AirStash_ExceptionCheck__throwAny(env) || !name)
        goto fail;

    ret_object = AirStashC_MediaCodec__createByCodecName(env, name);
    if (AirStash_ExceptionCheck__throwAny(env) || !ret_object) {
        ret_object = NULL;
        goto fail;
    }

fail:
    AirStash_DeleteLocalRef__p(env, &name);
    return ret_object;
}

jobject AirStashC_MediaCodec__createByCodecName__withCString__catchAll(JNIEnv *env, const char *name_cstr__)
{
    jobject ret_object = NULL;
    jstring name = NULL;

    name = (*env)->NewStringUTF(env, name_cstr__);
    if (AirStash_ExceptionCheck__catchAll(env) || !name)
        goto fail;

    ret_object = AirStashC_MediaCodec__createByCodecName__catchAll(env, name);
    if (AirStash_ExceptionCheck__catchAll(env) || !ret_object) {
        ret_object = NULL;
        goto fail;
    }

fail:
    AirStash_DeleteLocalRef__p(env, &name);
    return ret_object;
}

jobject AirStashC_MediaCodec__createByCodecName__withCString__asGlobalRef__catchAll(JNIEnv *env, const char *name_cstr__)
{
    jobject ret_object   = NULL;
    jobject local_object = AirStashC_MediaCodec__createByCodecName__withCString__catchAll(env, name_cstr__);
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

void AirStashC_MediaCodec__configure(JNIEnv *env, jobject thiz, jobject format, jobject surface, jobject crypto, jint flags)
{
    (*env)->CallVoidMethod(env, thiz, class_AirStashC_MediaCodec.method_configure, format, surface, crypto, flags);
}

void AirStashC_MediaCodec__configure__catchAll(JNIEnv *env, jobject thiz, jobject format, jobject surface, jobject crypto, jint flags)
{
    AirStashC_MediaCodec__configure(env, thiz, format, surface, crypto, flags);
    AirStash_ExceptionCheck__catchAll(env);
}

jobject AirStashC_MediaCodec__getOutputFormat(JNIEnv *env, jobject thiz)
{
    return (*env)->CallObjectMethod(env, thiz, class_AirStashC_MediaCodec.method_getOutputFormat);
}

jobject AirStashC_MediaCodec__getOutputFormat__catchAll(JNIEnv *env, jobject thiz)
{
    jobject ret_object = AirStashC_MediaCodec__getOutputFormat(env, thiz);
    if (AirStash_ExceptionCheck__catchAll(env) || !ret_object) {
        return NULL;
    }

    return ret_object;
}

jobject AirStashC_MediaCodec__getOutputFormat__asGlobalRef__catchAll(JNIEnv *env, jobject thiz)
{
    jobject ret_object   = NULL;
    jobject local_object = AirStashC_MediaCodec__getOutputFormat__catchAll(env, thiz);
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

jobjectArray AirStashC_MediaCodec__getInputBuffers(JNIEnv *env, jobject thiz)
{
    return (*env)->CallObjectMethod(env, thiz, class_AirStashC_MediaCodec.method_getInputBuffers);
}

jobjectArray AirStashC_MediaCodec__getInputBuffers__catchAll(JNIEnv *env, jobject thiz)
{
    jobjectArray ret_object = AirStashC_MediaCodec__getInputBuffers(env, thiz);
    if (AirStash_ExceptionCheck__catchAll(env) || !ret_object) {
        return NULL;
    }

    return ret_object;
}

jobjectArray AirStashC_MediaCodec__getInputBuffers__asGlobalRef__catchAll(JNIEnv *env, jobject thiz)
{
    jobjectArray ret_object   = NULL;
    jobjectArray local_object = AirStashC_MediaCodec__getInputBuffers__catchAll(env, thiz);
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

jint AirStashC_MediaCodec__dequeueInputBuffer(JNIEnv *env, jobject thiz, jlong timeoutUs)
{
    return (*env)->CallIntMethod(env, thiz, class_AirStashC_MediaCodec.method_dequeueInputBuffer, timeoutUs);
}

jint AirStashC_MediaCodec__dequeueInputBuffer__catchAll(JNIEnv *env, jobject thiz, jlong timeoutUs)
{
    jint ret_value = AirStashC_MediaCodec__dequeueInputBuffer(env, thiz, timeoutUs);
    if (AirStash_ExceptionCheck__catchAll(env)) {
        return 0;
    }

    return ret_value;
}

void AirStashC_MediaCodec__queueInputBuffer(JNIEnv *env, jobject thiz, jint index, jint offset, jint size, jlong presentationTimeUs, jint flags)
{
    (*env)->CallVoidMethod(env, thiz, class_AirStashC_MediaCodec.method_queueInputBuffer, index, offset, size, presentationTimeUs, flags);
}

void AirStashC_MediaCodec__queueInputBuffer__catchAll(JNIEnv *env, jobject thiz, jint index, jint offset, jint size, jlong presentationTimeUs, jint flags)
{
    AirStashC_MediaCodec__queueInputBuffer(env, thiz, index, offset, size, presentationTimeUs, flags);
    AirStash_ExceptionCheck__catchAll(env);
}

jint AirStashC_MediaCodec__dequeueOutputBuffer(JNIEnv *env, jobject thiz, jobject info, jlong timeoutUs)
{
    return (*env)->CallIntMethod(env, thiz, class_AirStashC_MediaCodec.method_dequeueOutputBuffer, info, timeoutUs);
}

jint AirStashC_MediaCodec__dequeueOutputBuffer__catchAll(JNIEnv *env, jobject thiz, jobject info, jlong timeoutUs)
{
    jint ret_value = AirStashC_MediaCodec__dequeueOutputBuffer(env, thiz, info, timeoutUs);
    if (AirStash_ExceptionCheck__catchAll(env)) {
        return 0;
    }

    return ret_value;
}

void AirStashC_MediaCodec__releaseOutputBuffer(JNIEnv *env, jobject thiz, jint index, jboolean render)
{
    (*env)->CallVoidMethod(env, thiz, class_AirStashC_MediaCodec.method_releaseOutputBuffer, index, render);
}

void AirStashC_MediaCodec__releaseOutputBuffer__catchAll(JNIEnv *env, jobject thiz, jint index, jboolean render)
{
    AirStashC_MediaCodec__releaseOutputBuffer(env, thiz, index, render);
    AirStash_ExceptionCheck__catchAll(env);
}

void AirStashC_MediaCodec__start(JNIEnv *env, jobject thiz)
{
    (*env)->CallVoidMethod(env, thiz, class_AirStashC_MediaCodec.method_start);
}

void AirStashC_MediaCodec__start__catchAll(JNIEnv *env, jobject thiz)
{
    AirStashC_MediaCodec__start(env, thiz);
    AirStash_ExceptionCheck__catchAll(env);
}

void AirStashC_MediaCodec__stop(JNIEnv *env, jobject thiz)
{
    (*env)->CallVoidMethod(env, thiz, class_AirStashC_MediaCodec.method_stop);
}

void AirStashC_MediaCodec__stop__catchAll(JNIEnv *env, jobject thiz)
{
    AirStashC_MediaCodec__stop(env, thiz);
    AirStash_ExceptionCheck__catchAll(env);
}

void AirStashC_MediaCodec__flush(JNIEnv *env, jobject thiz)
{
    (*env)->CallVoidMethod(env, thiz, class_AirStashC_MediaCodec.method_flush);
}

void AirStashC_MediaCodec__flush__catchAll(JNIEnv *env, jobject thiz)
{
    AirStashC_MediaCodec__flush(env, thiz);
    AirStash_ExceptionCheck__catchAll(env);
}

void AirStashC_MediaCodec__release(JNIEnv *env, jobject thiz)
{
    (*env)->CallVoidMethod(env, thiz, class_AirStashC_MediaCodec.method_release);
}

void AirStashC_MediaCodec__release__catchAll(JNIEnv *env, jobject thiz)
{
    AirStashC_MediaCodec__release(env, thiz);
    AirStash_ExceptionCheck__catchAll(env);
}

int AirStash_loadClass__AirStashC_MediaCodec(JNIEnv *env)
{
    int         ret                   = -1;
    const char *AIRSTASH_UNUSED(name)      = NULL;
    const char *AIRSTASH_UNUSED(sign)      = NULL;
    jclass      AIRSTASH_UNUSED(class_id)  = NULL;
    int         AIRSTASH_UNUSED(api_level) = 0;

    api_level = AirStash_GetSystemAndroidApiLevel(env);

    if (api_level < 16) {
        ALOGW("AirStashLoader: Ignore: '%s' need API %d\n", "android.media.MediaCodec", api_level);
        goto ignore;
    }

    sign = "android/media/MediaCodec";
    class_AirStashC_MediaCodec.id = AirStash_FindClass__asGlobalRef__catchAll(env, sign);
    if (class_AirStashC_MediaCodec.id == NULL)
        goto fail;

    ret = AirStash_loadClass__AirStashC_MediaCodec__BufferInfo(env);
    if (ret)
        goto fail;

    class_id = class_AirStashC_MediaCodec.id;
    name     = "createByCodecName";
    sign     = "(Ljava/lang/String;)Landroid/media/MediaCodec;";
    class_AirStashC_MediaCodec.method_createByCodecName = AirStash_GetStaticMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_MediaCodec.method_createByCodecName == NULL)
        goto fail;

    class_id = class_AirStashC_MediaCodec.id;
    name     = "configure";
    sign     = "(Landroid/media/MediaFormat;Landroid/view/Surface;Landroid/media/MediaCrypto;I)V";
    class_AirStashC_MediaCodec.method_configure = AirStash_GetMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_MediaCodec.method_configure == NULL)
        goto fail;

    class_id = class_AirStashC_MediaCodec.id;
    name     = "getOutputFormat";
    sign     = "()Landroid/media/MediaFormat;";
    class_AirStashC_MediaCodec.method_getOutputFormat = AirStash_GetMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_MediaCodec.method_getOutputFormat == NULL)
        goto fail;

    class_id = class_AirStashC_MediaCodec.id;
    name     = "getInputBuffers";
    sign     = "()[Ljava/nio/ByteBuffer;";
    class_AirStashC_MediaCodec.method_getInputBuffers = AirStash_GetMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_MediaCodec.method_getInputBuffers == NULL)
        goto fail;

    class_id = class_AirStashC_MediaCodec.id;
    name     = "dequeueInputBuffer";
    sign     = "(J)I";
    class_AirStashC_MediaCodec.method_dequeueInputBuffer = AirStash_GetMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_MediaCodec.method_dequeueInputBuffer == NULL)
        goto fail;

    class_id = class_AirStashC_MediaCodec.id;
    name     = "queueInputBuffer";
    sign     = "(IIIJI)V";
    class_AirStashC_MediaCodec.method_queueInputBuffer = AirStash_GetMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_MediaCodec.method_queueInputBuffer == NULL)
        goto fail;

    class_id = class_AirStashC_MediaCodec.id;
    name     = "dequeueOutputBuffer";
    sign     = "(Landroid/media/MediaCodec$BufferInfo;J)I";
    class_AirStashC_MediaCodec.method_dequeueOutputBuffer = AirStash_GetMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_MediaCodec.method_dequeueOutputBuffer == NULL)
        goto fail;

    class_id = class_AirStashC_MediaCodec.id;
    name     = "releaseOutputBuffer";
    sign     = "(IZ)V";
    class_AirStashC_MediaCodec.method_releaseOutputBuffer = AirStash_GetMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_MediaCodec.method_releaseOutputBuffer == NULL)
        goto fail;

    class_id = class_AirStashC_MediaCodec.id;
    name     = "start";
    sign     = "()V";
    class_AirStashC_MediaCodec.method_start = AirStash_GetMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_MediaCodec.method_start == NULL)
        goto fail;

    class_id = class_AirStashC_MediaCodec.id;
    name     = "stop";
    sign     = "()V";
    class_AirStashC_MediaCodec.method_stop = AirStash_GetMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_MediaCodec.method_stop == NULL)
        goto fail;

    class_id = class_AirStashC_MediaCodec.id;
    name     = "flush";
    sign     = "()V";
    class_AirStashC_MediaCodec.method_flush = AirStash_GetMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_MediaCodec.method_flush == NULL)
        goto fail;

    class_id = class_AirStashC_MediaCodec.id;
    name     = "release";
    sign     = "()V";
    class_AirStashC_MediaCodec.method_release = AirStash_GetMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_MediaCodec.method_release == NULL)
        goto fail;

    ALOGD("AirStashLoader: OK: '%s' loaded\n", "android.media.MediaCodec");
ignore:
    ret = 0;
fail:
    return ret;
}
