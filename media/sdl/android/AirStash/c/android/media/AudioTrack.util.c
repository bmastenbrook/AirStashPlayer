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
#include "AudioTrack.util.h"
#include "PlaybackParams.h"

#ifdef AIRSTASH_HAVE__AirStashC_AudioTrack
#define CALL_METHOD(method__) AirStashC_AudioTrack__##method__
#else
#define CALL_METHOD(method__) AirStashC_android_media_AudioTrack__##method__
#endif

void AirStashC_android_media_AudioTrack__setSpeed(JNIEnv *env, jobject thiz, jfloat speed)
{
    if (AirStash_GetSystemAndroidApiLevel(env) < 23)
        return;

    jobject temp = NULL;
    jobject params = CALL_METHOD(getPlaybackParams)(env, thiz);
    if (AirStash_ExceptionCheck__throwAny(env) || !params)
        goto fail;

#ifdef AIRSTASH_HAVE__AirStashC_PlaybackParams
    temp = AirStashC_PlaybackParams__setSpeed(env, params, speed);
#else
    temp = AirStashC_android_media_PlaybackParams__setSpeed(env, params, speed);
#endif
    AirStash_DeleteLocalRef__p(env, &temp);
    if (AirStash_ExceptionCheck__throwAny(env))
        goto fail;

    ALOGE("%s %f", __func__, (double)speed);
    CALL_METHOD(setPlaybackParams)(env, thiz, params);
    if (AirStash_ExceptionCheck__throwAny(env))
        goto fail;

fail:
    AirStash_DeleteLocalRef__p(env, &params);
}

void AirStashC_android_media_AudioTrack__setSpeed__catchAll(JNIEnv *env, jobject thiz, jfloat speed)
{
    ALOGE("%s", __func__);
    AirStashC_android_media_AudioTrack__setSpeed(env, thiz, speed);
    if (AirStash_ExceptionCheck__catchAll(env))
        return;

    return;
}
