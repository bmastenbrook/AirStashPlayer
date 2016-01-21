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

#include "AirStashMediaPlayer.h"

typedef struct AirStashC_AirStashMediaPlayer {
    jclass id;

    jfieldID field_mNativeMediaPlayer;
    jfieldID field_mNativeMediaDataSource;
    jmethodID method_postEventFromNative;
    jmethodID method_onSelectCodec;
    jmethodID method_onNativeInvoke;
} AirStashC_AirStashMediaPlayer;
static AirStashC_AirStashMediaPlayer class_AirStashC_AirStashMediaPlayer;

jlong AirStashC_AirStashMediaPlayer__mNativeMediaPlayer__get(JNIEnv *env, jobject thiz)
{
    return (*env)->GetLongField(env, thiz, class_AirStashC_AirStashMediaPlayer.field_mNativeMediaPlayer);
}

jlong AirStashC_AirStashMediaPlayer__mNativeMediaPlayer__get__catchAll(JNIEnv *env, jobject thiz)
{
    jlong ret_value = AirStashC_AirStashMediaPlayer__mNativeMediaPlayer__get(env, thiz);
    if (AirStash_ExceptionCheck__catchAll(env)) {
        return 0;
    }

    return ret_value;
}

void AirStashC_AirStashMediaPlayer__mNativeMediaPlayer__set(JNIEnv *env, jobject thiz, jlong value)
{
    (*env)->SetLongField(env, thiz, class_AirStashC_AirStashMediaPlayer.field_mNativeMediaPlayer, value);
}

void AirStashC_AirStashMediaPlayer__mNativeMediaPlayer__set__catchAll(JNIEnv *env, jobject thiz, jlong value)
{
    AirStashC_AirStashMediaPlayer__mNativeMediaPlayer__set(env, thiz, value);
    AirStash_ExceptionCheck__catchAll(env);
}

jlong AirStashC_AirStashMediaPlayer__mNativeMediaDataSource__get(JNIEnv *env, jobject thiz)
{
    return (*env)->GetLongField(env, thiz, class_AirStashC_AirStashMediaPlayer.field_mNativeMediaDataSource);
}

jlong AirStashC_AirStashMediaPlayer__mNativeMediaDataSource__get__catchAll(JNIEnv *env, jobject thiz)
{
    jlong ret_value = AirStashC_AirStashMediaPlayer__mNativeMediaDataSource__get(env, thiz);
    if (AirStash_ExceptionCheck__catchAll(env)) {
        return 0;
    }

    return ret_value;
}

void AirStashC_AirStashMediaPlayer__mNativeMediaDataSource__set(JNIEnv *env, jobject thiz, jlong value)
{
    (*env)->SetLongField(env, thiz, class_AirStashC_AirStashMediaPlayer.field_mNativeMediaDataSource, value);
}

void AirStashC_AirStashMediaPlayer__mNativeMediaDataSource__set__catchAll(JNIEnv *env, jobject thiz, jlong value)
{
    AirStashC_AirStashMediaPlayer__mNativeMediaDataSource__set(env, thiz, value);
    AirStash_ExceptionCheck__catchAll(env);
}

void AirStashC_AirStashMediaPlayer__postEventFromNative(JNIEnv *env, jobject weakThiz, jint what, jint arg1, jint arg2, jobject obj)
{
    (*env)->CallStaticVoidMethod(env, class_AirStashC_AirStashMediaPlayer.id, class_AirStashC_AirStashMediaPlayer.method_postEventFromNative, weakThiz, what, arg1, arg2, obj);
}

void AirStashC_AirStashMediaPlayer__postEventFromNative__catchAll(JNIEnv *env, jobject weakThiz, jint what, jint arg1, jint arg2, jobject obj)
{
    AirStashC_AirStashMediaPlayer__postEventFromNative(env, weakThiz, what, arg1, arg2, obj);
    AirStash_ExceptionCheck__catchAll(env);
}

jstring AirStashC_AirStashMediaPlayer__onSelectCodec(JNIEnv *env, jobject weakThiz, jstring mimeType, jint profile, jint level)
{
    return (*env)->CallStaticObjectMethod(env, class_AirStashC_AirStashMediaPlayer.id, class_AirStashC_AirStashMediaPlayer.method_onSelectCodec, weakThiz, mimeType, profile, level);
}

jstring AirStashC_AirStashMediaPlayer__onSelectCodec__catchAll(JNIEnv *env, jobject weakThiz, jstring mimeType, jint profile, jint level)
{
    jstring ret_object = AirStashC_AirStashMediaPlayer__onSelectCodec(env, weakThiz, mimeType, profile, level);
    if (AirStash_ExceptionCheck__catchAll(env) || !ret_object) {
        return NULL;
    }

    return ret_object;
}

jstring AirStashC_AirStashMediaPlayer__onSelectCodec__asGlobalRef__catchAll(JNIEnv *env, jobject weakThiz, jstring mimeType, jint profile, jint level)
{
    jstring ret_object   = NULL;
    jstring local_object = AirStashC_AirStashMediaPlayer__onSelectCodec__catchAll(env, weakThiz, mimeType, profile, level);
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

const char *AirStashC_AirStashMediaPlayer__onSelectCodec__asCBuffer(JNIEnv *env, jobject weakThiz, jstring mimeType, jint profile, jint level, char *out_buf, int out_len)
{
    const char *ret_value = NULL;
    const char *c_str     = NULL;
    jstring local_string = AirStashC_AirStashMediaPlayer__onSelectCodec(env, weakThiz, mimeType, profile, level);
    if (AirStash_ExceptionCheck__throwAny(env) || !local_string) {
        goto fail;
    }

    c_str = (*env)->GetStringUTFChars(env, local_string, NULL );
    if (AirStash_ExceptionCheck__throwAny(env) || !c_str) {
        goto fail;
    }

    strlcpy(out_buf, c_str, out_len);
    ret_value = out_buf;

fail:
    AirStash_ReleaseStringUTFChars__p(env, local_string, &c_str);
    AirStash_DeleteLocalRef__p(env, &local_string);
    return ret_value;
}

const char *AirStashC_AirStashMediaPlayer__onSelectCodec__asCBuffer__catchAll(JNIEnv *env, jobject weakThiz, jstring mimeType, jint profile, jint level, char *out_buf, int out_len)
{
    const char *ret_value = NULL;
    const char *c_str     = NULL;
    jstring local_string = AirStashC_AirStashMediaPlayer__onSelectCodec__catchAll(env, weakThiz, mimeType, profile, level);
    if (AirStash_ExceptionCheck__catchAll(env) || !local_string) {
        goto fail;
    }

    c_str = (*env)->GetStringUTFChars(env, local_string, NULL );
    if (AirStash_ExceptionCheck__catchAll(env) || !c_str) {
        goto fail;
    }

    strlcpy(out_buf, c_str, out_len);
    ret_value = out_buf;

fail:
    AirStash_ReleaseStringUTFChars__p(env, local_string, &c_str);
    AirStash_DeleteLocalRef__p(env, &local_string);
    return ret_value;
}

jstring AirStashC_AirStashMediaPlayer__onSelectCodec__withCString(JNIEnv *env, jobject weakThiz, const char *mimeType_cstr__, jint profile, jint level)
{
    jstring ret_object = NULL;
    jstring mimeType = NULL;

    mimeType = (*env)->NewStringUTF(env, mimeType_cstr__);
    if (AirStash_ExceptionCheck__throwAny(env) || !mimeType)
        goto fail;

    ret_object = AirStashC_AirStashMediaPlayer__onSelectCodec(env, weakThiz, mimeType, profile, level);
    if (AirStash_ExceptionCheck__throwAny(env) || !ret_object) {
        ret_object = NULL;
        goto fail;
    }

fail:
    AirStash_DeleteLocalRef__p(env, &mimeType);
    return ret_object;
}

jstring AirStashC_AirStashMediaPlayer__onSelectCodec__withCString__catchAll(JNIEnv *env, jobject weakThiz, const char *mimeType_cstr__, jint profile, jint level)
{
    jstring ret_object = NULL;
    jstring mimeType = NULL;

    mimeType = (*env)->NewStringUTF(env, mimeType_cstr__);
    if (AirStash_ExceptionCheck__catchAll(env) || !mimeType)
        goto fail;

    ret_object = AirStashC_AirStashMediaPlayer__onSelectCodec__catchAll(env, weakThiz, mimeType, profile, level);
    if (AirStash_ExceptionCheck__catchAll(env) || !ret_object) {
        ret_object = NULL;
        goto fail;
    }

fail:
    AirStash_DeleteLocalRef__p(env, &mimeType);
    return ret_object;
}

jstring AirStashC_AirStashMediaPlayer__onSelectCodec__withCString__asGlobalRef__catchAll(JNIEnv *env, jobject weakThiz, const char *mimeType_cstr__, jint profile, jint level)
{
    jstring ret_object   = NULL;
    jstring local_object = AirStashC_AirStashMediaPlayer__onSelectCodec__withCString__catchAll(env, weakThiz, mimeType_cstr__, profile, level);
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

const char *AirStashC_AirStashMediaPlayer__onSelectCodec__withCString__asCBuffer(JNIEnv *env, jobject weakThiz, const char *mimeType_cstr__, jint profile, jint level, char *out_buf, int out_len)
{
    const char *ret_value = NULL;
    const char *c_str     = NULL;
    jstring local_string = AirStashC_AirStashMediaPlayer__onSelectCodec__withCString(env, weakThiz, mimeType_cstr__, profile, level);
    if (AirStash_ExceptionCheck__throwAny(env) || !local_string) {
        goto fail;
    }

    c_str = (*env)->GetStringUTFChars(env, local_string, NULL );
    if (AirStash_ExceptionCheck__throwAny(env) || !c_str) {
        goto fail;
    }

    strlcpy(out_buf, c_str, out_len);
    ret_value = out_buf;

fail:
    AirStash_ReleaseStringUTFChars__p(env, local_string, &c_str);
    AirStash_DeleteLocalRef__p(env, &local_string);
    return ret_value;
}

const char *AirStashC_AirStashMediaPlayer__onSelectCodec__withCString__asCBuffer__catchAll(JNIEnv *env, jobject weakThiz, const char *mimeType_cstr__, jint profile, jint level, char *out_buf, int out_len)
{
    const char *ret_value = NULL;
    const char *c_str     = NULL;
    jstring local_string = AirStashC_AirStashMediaPlayer__onSelectCodec__withCString__catchAll(env, weakThiz, mimeType_cstr__, profile, level);
    if (AirStash_ExceptionCheck__catchAll(env) || !local_string) {
        goto fail;
    }

    c_str = (*env)->GetStringUTFChars(env, local_string, NULL );
    if (AirStash_ExceptionCheck__catchAll(env) || !c_str) {
        goto fail;
    }

    strlcpy(out_buf, c_str, out_len);
    ret_value = out_buf;

fail:
    AirStash_ReleaseStringUTFChars__p(env, local_string, &c_str);
    AirStash_DeleteLocalRef__p(env, &local_string);
    return ret_value;
}

jboolean AirStashC_AirStashMediaPlayer__onNativeInvoke(JNIEnv *env, jobject weakThiz, jint what, jobject args)
{
    return (*env)->CallStaticBooleanMethod(env, class_AirStashC_AirStashMediaPlayer.id, class_AirStashC_AirStashMediaPlayer.method_onNativeInvoke, weakThiz, what, args);
}

jboolean AirStashC_AirStashMediaPlayer__onNativeInvoke__catchAll(JNIEnv *env, jobject weakThiz, jint what, jobject args)
{
    jboolean ret_value = AirStashC_AirStashMediaPlayer__onNativeInvoke(env, weakThiz, what, args);
    if (AirStash_ExceptionCheck__catchAll(env)) {
        return false;
    }

    return ret_value;
}

int AirStash_loadClass__AirStashC_AirStashMediaPlayer(JNIEnv *env)
{
    int         ret                   = -1;
    const char *AIRSTASH_UNUSED(name)      = NULL;
    const char *AIRSTASH_UNUSED(sign)      = NULL;
    jclass      AIRSTASH_UNUSED(class_id)  = NULL;
    int         AIRSTASH_UNUSED(api_level) = 0;

    sign = "com/AirStash/player/player/AirStashMediaPlayer";
    class_AirStashC_AirStashMediaPlayer.id = AirStash_FindClass__asGlobalRef__catchAll(env, sign);
    if (class_AirStashC_AirStashMediaPlayer.id == NULL)
        goto fail;

    class_id = class_AirStashC_AirStashMediaPlayer.id;
    name     = "mNativeMediaPlayer";
    sign     = "J";
    class_AirStashC_AirStashMediaPlayer.field_mNativeMediaPlayer = AirStash_GetFieldID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_AirStashMediaPlayer.field_mNativeMediaPlayer == NULL)
        goto fail;

    class_id = class_AirStashC_AirStashMediaPlayer.id;
    name     = "mNativeMediaDataSource";
    sign     = "J";
    class_AirStashC_AirStashMediaPlayer.field_mNativeMediaDataSource = AirStash_GetFieldID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_AirStashMediaPlayer.field_mNativeMediaDataSource == NULL)
        goto fail;

    class_id = class_AirStashC_AirStashMediaPlayer.id;
    name     = "postEventFromNative";
    sign     = "(Ljava/lang/Object;IIILjava/lang/Object;)V";
    class_AirStashC_AirStashMediaPlayer.method_postEventFromNative = AirStash_GetStaticMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_AirStashMediaPlayer.method_postEventFromNative == NULL)
        goto fail;

    class_id = class_AirStashC_AirStashMediaPlayer.id;
    name     = "onSelectCodec";
    sign     = "(Ljava/lang/Object;Ljava/lang/String;II)Ljava/lang/String;";
    class_AirStashC_AirStashMediaPlayer.method_onSelectCodec = AirStash_GetStaticMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_AirStashMediaPlayer.method_onSelectCodec == NULL)
        goto fail;

    class_id = class_AirStashC_AirStashMediaPlayer.id;
    name     = "onNativeInvoke";
    sign     = "(Ljava/lang/Object;ILandroid/os/Bundle;)Z";
    class_AirStashC_AirStashMediaPlayer.method_onNativeInvoke = AirStash_GetStaticMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_AirStashMediaPlayer.method_onNativeInvoke == NULL)
        goto fail;

    ALOGD("AirStashLoader: OK: '%s' loaded\n", "com.AirStash.player.player.AirStashMediaPlayer");
    ret = 0;
fail:
    return ret;
}
