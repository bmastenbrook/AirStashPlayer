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

#include "ByteBuffer.h"

typedef struct AirStashC_ByteBuffer {
    jclass id;

    jmethodID method_allocate;
    jmethodID method_allocateDirect;
    jmethodID method_limit;
} AirStashC_ByteBuffer;
static AirStashC_ByteBuffer class_AirStashC_ByteBuffer;

jobject AirStashC_ByteBuffer__allocate(JNIEnv *env, jint capacity)
{
    return (*env)->CallStaticObjectMethod(env, class_AirStashC_ByteBuffer.id, class_AirStashC_ByteBuffer.method_allocate, capacity);
}

jobject AirStashC_ByteBuffer__allocate__catchAll(JNIEnv *env, jint capacity)
{
    jobject ret_object = AirStashC_ByteBuffer__allocate(env, capacity);
    if (AirStash_ExceptionCheck__catchAll(env) || !ret_object) {
        return NULL;
    }

    return ret_object;
}

jobject AirStashC_ByteBuffer__allocate__asGlobalRef__catchAll(JNIEnv *env, jint capacity)
{
    jobject ret_object   = NULL;
    jobject local_object = AirStashC_ByteBuffer__allocate__catchAll(env, capacity);
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

jobject AirStashC_ByteBuffer__allocateDirect(JNIEnv *env, jint capacity)
{
    return (*env)->CallStaticObjectMethod(env, class_AirStashC_ByteBuffer.id, class_AirStashC_ByteBuffer.method_allocateDirect, capacity);
}

jobject AirStashC_ByteBuffer__allocateDirect__catchAll(JNIEnv *env, jint capacity)
{
    jobject ret_object = AirStashC_ByteBuffer__allocateDirect(env, capacity);
    if (AirStash_ExceptionCheck__catchAll(env) || !ret_object) {
        return NULL;
    }

    return ret_object;
}

jobject AirStashC_ByteBuffer__allocateDirect__asGlobalRef__catchAll(JNIEnv *env, jint capacity)
{
    jobject ret_object   = NULL;
    jobject local_object = AirStashC_ByteBuffer__allocateDirect__catchAll(env, capacity);
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

jobject AirStashC_ByteBuffer__limit(JNIEnv *env, jobject thiz, jint newLimit)
{
    return (*env)->CallObjectMethod(env, thiz, class_AirStashC_ByteBuffer.method_limit, newLimit);
}

jobject AirStashC_ByteBuffer__limit__catchAll(JNIEnv *env, jobject thiz, jint newLimit)
{
    jobject ret_object = AirStashC_ByteBuffer__limit(env, thiz, newLimit);
    if (AirStash_ExceptionCheck__catchAll(env) || !ret_object) {
        return NULL;
    }

    return ret_object;
}

jobject AirStashC_ByteBuffer__limit__asGlobalRef__catchAll(JNIEnv *env, jobject thiz, jint newLimit)
{
    jobject ret_object   = NULL;
    jobject local_object = AirStashC_ByteBuffer__limit__catchAll(env, thiz, newLimit);
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

int AirStash_loadClass__AirStashC_ByteBuffer(JNIEnv *env)
{
    int         ret                   = -1;
    const char *AIRSTASH_UNUSED(name)      = NULL;
    const char *AIRSTASH_UNUSED(sign)      = NULL;
    jclass      AIRSTASH_UNUSED(class_id)  = NULL;
    int         AIRSTASH_UNUSED(api_level) = 0;

    sign = "java/nio/ByteBuffer";
    class_AirStashC_ByteBuffer.id = AirStash_FindClass__asGlobalRef__catchAll(env, sign);
    if (class_AirStashC_ByteBuffer.id == NULL)
        goto fail;

    class_id = class_AirStashC_ByteBuffer.id;
    name     = "allocate";
    sign     = "(I)Ljava/nio/ByteBuffer;";
    class_AirStashC_ByteBuffer.method_allocate = AirStash_GetStaticMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_ByteBuffer.method_allocate == NULL)
        goto fail;

    class_id = class_AirStashC_ByteBuffer.id;
    name     = "allocateDirect";
    sign     = "(I)Ljava/nio/ByteBuffer;";
    class_AirStashC_ByteBuffer.method_allocateDirect = AirStash_GetStaticMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_ByteBuffer.method_allocateDirect == NULL)
        goto fail;

    class_id = class_AirStashC_ByteBuffer.id;
    name     = "limit";
    sign     = "(I)Ljava/nio/Buffer;";
    class_AirStashC_ByteBuffer.method_limit = AirStash_GetMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_ByteBuffer.method_limit == NULL)
        goto fail;

    ALOGD("AirStashLoader: OK: '%s' loaded\n", "java.nio.ByteBuffer");
    ret = 0;
fail:
    return ret;
}
