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
#include "ByteBuffer.util.h"
#include <string.h>

#ifdef AIRSTASH_HAVE__AirStashC_ByteBuffer
#define CALL_METHOD(method__) AirStashC_ByteBuffer__##method__
#else
#define CALL_METHOD(method__) AirStashC_java_nio_ByteBuffer__##method__
#endif

void *AirStashC_java_nio_ByteBuffer__getDirectBufferAddress(JNIEnv *env, jobject thiz)
{
    return (*env)->GetDirectBufferAddress(env, thiz);
}

void *AirStashC_java_nio_ByteBuffer__getDirectBufferAddress__catchAll(JNIEnv *env, jobject thiz)
{
    void *ret = (*env)->GetDirectBufferAddress(env, thiz);
    if (AirStash_ExceptionCheck__catchAll(env) || !ret)
        return NULL;

    return ret;
}

int AirStashC_java_nio_ByteBuffer__assignData__catchAll(JNIEnv *env, jobject thiz, void* data, size_t size)
{
    jobject buffer = CALL_METHOD(limit)(env, thiz, size);
    if (AirStash_ExceptionCheck__catchAll(env) || !buffer)
        return -1;
    AirStash_DeleteLocalRef__p(env, &buffer);

    uint8_t *c_buffer = AirStashC_java_nio_ByteBuffer__getDirectBufferAddress__catchAll(env, thiz);
    if (!c_buffer)
        return -1;

    memcpy(c_buffer, data, size);
    return 0;
}
