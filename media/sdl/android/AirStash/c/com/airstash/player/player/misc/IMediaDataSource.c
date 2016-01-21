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
#include "IMediaDataSource.h"

typedef struct AirStashC_IMediaDataSource {
    jclass id;

    jmethodID method_readAt;
    jmethodID method_getSize;
    jmethodID method_close;
} AirStashC_IMediaDataSource;
static AirStashC_IMediaDataSource class_AirStashC_IMediaDataSource;

jint AirStashC_IMediaDataSource__readAt(JNIEnv *env, jobject thiz, jlong position, jbyteArray buffer, jint offset, jint size)
{
    return (*env)->CallIntMethod(env, thiz, class_AirStashC_IMediaDataSource.method_readAt, position, buffer, offset, size);
}

jint AirStashC_IMediaDataSource__readAt__catchAll(JNIEnv *env, jobject thiz, jlong position, jbyteArray buffer, jint offset, jint size)
{
    jint ret_value = AirStashC_IMediaDataSource__readAt(env, thiz, position, buffer, offset, size);
    if (AirStash_ExceptionCheck__catchAll(env)) {
        return 0;
    }

    return ret_value;
}

jlong AirStashC_IMediaDataSource__getSize(JNIEnv *env, jobject thiz)
{
    return (*env)->CallLongMethod(env, thiz, class_AirStashC_IMediaDataSource.method_getSize);
}

jlong AirStashC_IMediaDataSource__getSize__catchAll(JNIEnv *env, jobject thiz)
{
    jlong ret_value = AirStashC_IMediaDataSource__getSize(env, thiz);
    if (AirStash_ExceptionCheck__catchAll(env)) {
        return 0;
    }

    return ret_value;
}

void AirStashC_IMediaDataSource__close(JNIEnv *env, jobject thiz)
{
    (*env)->CallVoidMethod(env, thiz, class_AirStashC_IMediaDataSource.method_close);
}

void AirStashC_IMediaDataSource__close__catchAll(JNIEnv *env, jobject thiz)
{
    AirStashC_IMediaDataSource__close(env, thiz);
    AirStash_ExceptionCheck__catchAll(env);
}

int AirStash_loadClass__AirStashC_IMediaDataSource(JNIEnv *env)
{
    int         ret                   = -1;
    const char *AIRSTASH_UNUSED(name)      = NULL;
    const char *AIRSTASH_UNUSED(sign)      = NULL;
    jclass      AIRSTASH_UNUSED(class_id)  = NULL;
    int         AIRSTASH_UNUSED(api_level) = 0;

    sign = "com/AirStash/player/player/misc/IMediaDataSource";
    class_AirStashC_IMediaDataSource.id = AirStash_FindClass__asGlobalRef__catchAll(env, sign);
    if (class_AirStashC_IMediaDataSource.id == NULL)
        goto fail;

    class_id = class_AirStashC_IMediaDataSource.id;
    name     = "readAt";
    sign     = "(J[BII)I";
    class_AirStashC_IMediaDataSource.method_readAt = AirStash_GetMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_IMediaDataSource.method_readAt == NULL)
        goto fail;

    class_id = class_AirStashC_IMediaDataSource.id;
    name     = "getSize";
    sign     = "()J";
    class_AirStashC_IMediaDataSource.method_getSize = AirStash_GetMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_IMediaDataSource.method_getSize == NULL)
        goto fail;

    class_id = class_AirStashC_IMediaDataSource.id;
    name     = "close";
    sign     = "()V";
    class_AirStashC_IMediaDataSource.method_close = AirStash_GetMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_IMediaDataSource.method_close == NULL)
        goto fail;

    ALOGD("AirStashLoader: OK: '%s' loaded\n", "com.AirStash.player.player.misc.IMediaDataSource");
    ret = 0;
fail:
    return ret;
}
