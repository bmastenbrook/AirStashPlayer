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

#include "AudioTrack.h"

typedef struct AirStashC_AudioTrack {
    jclass id;

    jmethodID constructor_AudioTrack;
    jmethodID method_getMinBufferSize;
    jmethodID method_getMaxVolume;
    jmethodID method_getMinVolume;
    jmethodID method_getNativeOutputSampleRate;
    jmethodID method_play;
    jmethodID method_pause;
    jmethodID method_stop;
    jmethodID method_flush;
    jmethodID method_release;
    jmethodID method_write;
    jmethodID method_setStereoVolume;
    jmethodID method_getAudioSessionId;
    jmethodID method_getPlaybackParams;
    jmethodID method_setPlaybackParams;
} AirStashC_AudioTrack;
static AirStashC_AudioTrack class_AirStashC_AudioTrack;

jobject AirStashC_AudioTrack__AudioTrack(JNIEnv *env, jint streamType, jint sampleRateInHz, jint channelConfig, jint audioFormat, jint bufferSizeInBytes, jint mode)
{
    return (*env)->NewObject(env, class_AirStashC_AudioTrack.id, class_AirStashC_AudioTrack.constructor_AudioTrack, streamType, sampleRateInHz, channelConfig, audioFormat, bufferSizeInBytes, mode);
}

jobject AirStashC_AudioTrack__AudioTrack__catchAll(JNIEnv *env, jint streamType, jint sampleRateInHz, jint channelConfig, jint audioFormat, jint bufferSizeInBytes, jint mode)
{
    jobject ret_object = AirStashC_AudioTrack__AudioTrack(env, streamType, sampleRateInHz, channelConfig, audioFormat, bufferSizeInBytes, mode);
    if (AirStash_ExceptionCheck__catchAll(env) || !ret_object) {
        return NULL;
    }

    return ret_object;
}

jobject AirStashC_AudioTrack__AudioTrack__asGlobalRef__catchAll(JNIEnv *env, jint streamType, jint sampleRateInHz, jint channelConfig, jint audioFormat, jint bufferSizeInBytes, jint mode)
{
    jobject ret_object   = NULL;
    jobject local_object = AirStashC_AudioTrack__AudioTrack__catchAll(env, streamType, sampleRateInHz, channelConfig, audioFormat, bufferSizeInBytes, mode);
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

jint AirStashC_AudioTrack__getMinBufferSize(JNIEnv *env, jint sampleRateInHz, jint channelConfig, jint audioFormat)
{
    return (*env)->CallStaticIntMethod(env, class_AirStashC_AudioTrack.id, class_AirStashC_AudioTrack.method_getMinBufferSize, sampleRateInHz, channelConfig, audioFormat);
}

jint AirStashC_AudioTrack__getMinBufferSize__catchAll(JNIEnv *env, jint sampleRateInHz, jint channelConfig, jint audioFormat)
{
    jint ret_value = AirStashC_AudioTrack__getMinBufferSize(env, sampleRateInHz, channelConfig, audioFormat);
    if (AirStash_ExceptionCheck__catchAll(env)) {
        return 0;
    }

    return ret_value;
}

jfloat AirStashC_AudioTrack__getMaxVolume(JNIEnv *env)
{
    return (*env)->CallStaticFloatMethod(env, class_AirStashC_AudioTrack.id, class_AirStashC_AudioTrack.method_getMaxVolume);
}

jfloat AirStashC_AudioTrack__getMaxVolume__catchAll(JNIEnv *env)
{
    jfloat ret_value = AirStashC_AudioTrack__getMaxVolume(env);
    if (AirStash_ExceptionCheck__catchAll(env)) {
        return 0;
    }

    return ret_value;
}

jfloat AirStashC_AudioTrack__getMinVolume(JNIEnv *env)
{
    return (*env)->CallStaticFloatMethod(env, class_AirStashC_AudioTrack.id, class_AirStashC_AudioTrack.method_getMinVolume);
}

jfloat AirStashC_AudioTrack__getMinVolume__catchAll(JNIEnv *env)
{
    jfloat ret_value = AirStashC_AudioTrack__getMinVolume(env);
    if (AirStash_ExceptionCheck__catchAll(env)) {
        return 0;
    }

    return ret_value;
}

jint AirStashC_AudioTrack__getNativeOutputSampleRate(JNIEnv *env, jint streamType)
{
    return (*env)->CallStaticIntMethod(env, class_AirStashC_AudioTrack.id, class_AirStashC_AudioTrack.method_getNativeOutputSampleRate, streamType);
}

jint AirStashC_AudioTrack__getNativeOutputSampleRate__catchAll(JNIEnv *env, jint streamType)
{
    jint ret_value = AirStashC_AudioTrack__getNativeOutputSampleRate(env, streamType);
    if (AirStash_ExceptionCheck__catchAll(env)) {
        return 0;
    }

    return ret_value;
}

void AirStashC_AudioTrack__play(JNIEnv *env, jobject thiz)
{
    (*env)->CallVoidMethod(env, thiz, class_AirStashC_AudioTrack.method_play);
}

void AirStashC_AudioTrack__play__catchAll(JNIEnv *env, jobject thiz)
{
    AirStashC_AudioTrack__play(env, thiz);
    AirStash_ExceptionCheck__catchAll(env);
}

void AirStashC_AudioTrack__pause(JNIEnv *env, jobject thiz)
{
    (*env)->CallVoidMethod(env, thiz, class_AirStashC_AudioTrack.method_pause);
}

void AirStashC_AudioTrack__pause__catchAll(JNIEnv *env, jobject thiz)
{
    AirStashC_AudioTrack__pause(env, thiz);
    AirStash_ExceptionCheck__catchAll(env);
}

void AirStashC_AudioTrack__stop(JNIEnv *env, jobject thiz)
{
    (*env)->CallVoidMethod(env, thiz, class_AirStashC_AudioTrack.method_stop);
}

void AirStashC_AudioTrack__stop__catchAll(JNIEnv *env, jobject thiz)
{
    AirStashC_AudioTrack__stop(env, thiz);
    AirStash_ExceptionCheck__catchAll(env);
}

void AirStashC_AudioTrack__flush(JNIEnv *env, jobject thiz)
{
    (*env)->CallVoidMethod(env, thiz, class_AirStashC_AudioTrack.method_flush);
}

void AirStashC_AudioTrack__flush__catchAll(JNIEnv *env, jobject thiz)
{
    AirStashC_AudioTrack__flush(env, thiz);
    AirStash_ExceptionCheck__catchAll(env);
}

void AirStashC_AudioTrack__release(JNIEnv *env, jobject thiz)
{
    (*env)->CallVoidMethod(env, thiz, class_AirStashC_AudioTrack.method_release);
}

void AirStashC_AudioTrack__release__catchAll(JNIEnv *env, jobject thiz)
{
    AirStashC_AudioTrack__release(env, thiz);
    AirStash_ExceptionCheck__catchAll(env);
}

jint AirStashC_AudioTrack__write(JNIEnv *env, jobject thiz, jbyteArray audioData, jint offsetInBytes, jint sizeInBytes)
{
    return (*env)->CallIntMethod(env, thiz, class_AirStashC_AudioTrack.method_write, audioData, offsetInBytes, sizeInBytes);
}

jint AirStashC_AudioTrack__write__catchAll(JNIEnv *env, jobject thiz, jbyteArray audioData, jint offsetInBytes, jint sizeInBytes)
{
    jint ret_value = AirStashC_AudioTrack__write(env, thiz, audioData, offsetInBytes, sizeInBytes);
    if (AirStash_ExceptionCheck__catchAll(env)) {
        return 0;
    }

    return ret_value;
}

jint AirStashC_AudioTrack__setStereoVolume(JNIEnv *env, jobject thiz, jfloat leftGain, jfloat rightGain)
{
    return (*env)->CallIntMethod(env, thiz, class_AirStashC_AudioTrack.method_setStereoVolume, leftGain, rightGain);
}

jint AirStashC_AudioTrack__setStereoVolume__catchAll(JNIEnv *env, jobject thiz, jfloat leftGain, jfloat rightGain)
{
    jint ret_value = AirStashC_AudioTrack__setStereoVolume(env, thiz, leftGain, rightGain);
    if (AirStash_ExceptionCheck__catchAll(env)) {
        return 0;
    }

    return ret_value;
}

jint AirStashC_AudioTrack__getAudioSessionId(JNIEnv *env, jobject thiz)
{
    return (*env)->CallIntMethod(env, thiz, class_AirStashC_AudioTrack.method_getAudioSessionId);
}

jint AirStashC_AudioTrack__getAudioSessionId__catchAll(JNIEnv *env, jobject thiz)
{
    jint ret_value = AirStashC_AudioTrack__getAudioSessionId(env, thiz);
    if (AirStash_ExceptionCheck__catchAll(env)) {
        return 0;
    }

    return ret_value;
}

jobject AirStashC_AudioTrack__getPlaybackParams(JNIEnv *env, jobject thiz)
{
    return (*env)->CallObjectMethod(env, thiz, class_AirStashC_AudioTrack.method_getPlaybackParams);
}

jobject AirStashC_AudioTrack__getPlaybackParams__catchAll(JNIEnv *env, jobject thiz)
{
    jobject ret_object = AirStashC_AudioTrack__getPlaybackParams(env, thiz);
    if (AirStash_ExceptionCheck__catchAll(env) || !ret_object) {
        return NULL;
    }

    return ret_object;
}

jobject AirStashC_AudioTrack__getPlaybackParams__asGlobalRef__catchAll(JNIEnv *env, jobject thiz)
{
    jobject ret_object   = NULL;
    jobject local_object = AirStashC_AudioTrack__getPlaybackParams__catchAll(env, thiz);
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

void AirStashC_AudioTrack__setPlaybackParams(JNIEnv *env, jobject thiz, jobject params)
{
    (*env)->CallVoidMethod(env, thiz, class_AirStashC_AudioTrack.method_setPlaybackParams, params);
}

void AirStashC_AudioTrack__setPlaybackParams__catchAll(JNIEnv *env, jobject thiz, jobject params)
{
    AirStashC_AudioTrack__setPlaybackParams(env, thiz, params);
    AirStash_ExceptionCheck__catchAll(env);
}

int AirStash_loadClass__AirStashC_AudioTrack(JNIEnv *env)
{
    int         ret                   = -1;
    const char *AIRSTASH_UNUSED(name)      = NULL;
    const char *AIRSTASH_UNUSED(sign)      = NULL;
    jclass      AIRSTASH_UNUSED(class_id)  = NULL;
    int         AIRSTASH_UNUSED(api_level) = 0;

    sign = "android/media/AudioTrack";
    class_AirStashC_AudioTrack.id = AirStash_FindClass__asGlobalRef__catchAll(env, sign);
    if (class_AirStashC_AudioTrack.id == NULL)
        goto fail;

    class_id = class_AirStashC_AudioTrack.id;
    name     = "<init>";
    sign     = "(IIIIII)V";
    class_AirStashC_AudioTrack.constructor_AudioTrack = AirStash_GetMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_AudioTrack.constructor_AudioTrack == NULL)
        goto fail;

    class_id = class_AirStashC_AudioTrack.id;
    name     = "getMinBufferSize";
    sign     = "(III)I";
    class_AirStashC_AudioTrack.method_getMinBufferSize = AirStash_GetStaticMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_AudioTrack.method_getMinBufferSize == NULL)
        goto fail;

    class_id = class_AirStashC_AudioTrack.id;
    name     = "getMaxVolume";
    sign     = "()F";
    class_AirStashC_AudioTrack.method_getMaxVolume = AirStash_GetStaticMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_AudioTrack.method_getMaxVolume == NULL)
        goto fail;

    class_id = class_AirStashC_AudioTrack.id;
    name     = "getMinVolume";
    sign     = "()F";
    class_AirStashC_AudioTrack.method_getMinVolume = AirStash_GetStaticMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_AudioTrack.method_getMinVolume == NULL)
        goto fail;

    class_id = class_AirStashC_AudioTrack.id;
    name     = "getNativeOutputSampleRate";
    sign     = "(I)I";
    class_AirStashC_AudioTrack.method_getNativeOutputSampleRate = AirStash_GetStaticMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_AudioTrack.method_getNativeOutputSampleRate == NULL)
        goto fail;

    class_id = class_AirStashC_AudioTrack.id;
    name     = "play";
    sign     = "()V";
    class_AirStashC_AudioTrack.method_play = AirStash_GetMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_AudioTrack.method_play == NULL)
        goto fail;

    class_id = class_AirStashC_AudioTrack.id;
    name     = "pause";
    sign     = "()V";
    class_AirStashC_AudioTrack.method_pause = AirStash_GetMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_AudioTrack.method_pause == NULL)
        goto fail;

    class_id = class_AirStashC_AudioTrack.id;
    name     = "stop";
    sign     = "()V";
    class_AirStashC_AudioTrack.method_stop = AirStash_GetMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_AudioTrack.method_stop == NULL)
        goto fail;

    class_id = class_AirStashC_AudioTrack.id;
    name     = "flush";
    sign     = "()V";
    class_AirStashC_AudioTrack.method_flush = AirStash_GetMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_AudioTrack.method_flush == NULL)
        goto fail;

    class_id = class_AirStashC_AudioTrack.id;
    name     = "release";
    sign     = "()V";
    class_AirStashC_AudioTrack.method_release = AirStash_GetMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_AudioTrack.method_release == NULL)
        goto fail;

    class_id = class_AirStashC_AudioTrack.id;
    name     = "write";
    sign     = "([BII)I";
    class_AirStashC_AudioTrack.method_write = AirStash_GetMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_AudioTrack.method_write == NULL)
        goto fail;

    class_id = class_AirStashC_AudioTrack.id;
    name     = "setStereoVolume";
    sign     = "(FF)I";
    class_AirStashC_AudioTrack.method_setStereoVolume = AirStash_GetMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_AudioTrack.method_setStereoVolume == NULL)
        goto fail;

    class_id = class_AirStashC_AudioTrack.id;
    name     = "getAudioSessionId";
    sign     = "()I";
    class_AirStashC_AudioTrack.method_getAudioSessionId = AirStash_GetMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_AudioTrack.method_getAudioSessionId == NULL)
        goto fail;

    if (AirStash_GetSystemAndroidApiLevel(env) >= 23) {
        class_id = class_AirStashC_AudioTrack.id;
        name     = "getPlaybackParams";
        sign     = "()Landroid/media/PlaybackParams;";
        class_AirStashC_AudioTrack.method_getPlaybackParams = AirStash_GetMethodID__catchAll(env, class_id, name, sign);
        if (class_AirStashC_AudioTrack.method_getPlaybackParams == NULL)
            goto fail;
    }

    if (AirStash_GetSystemAndroidApiLevel(env) >= 23) {
        class_id = class_AirStashC_AudioTrack.id;
        name     = "setPlaybackParams";
        sign     = "(Landroid/media/PlaybackParams;)V";
        class_AirStashC_AudioTrack.method_setPlaybackParams = AirStash_GetMethodID__catchAll(env, class_id, name, sign);
        if (class_AirStashC_AudioTrack.method_setPlaybackParams == NULL)
            goto fail;
    }

    ALOGD("AirStashLoader: OK: '%s' loaded\n", "android.media.AudioTrack");
    ret = 0;
fail:
    return ret;
}
