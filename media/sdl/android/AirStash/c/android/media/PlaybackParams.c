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

#include "PlaybackParams.h"

typedef struct AirStashC_PlaybackParams {
    jclass id;

    jmethodID method_setSpeed;
} AirStashC_PlaybackParams;
static AirStashC_PlaybackParams class_AirStashC_PlaybackParams;

jobject AirStashC_PlaybackParams__setSpeed(JNIEnv *env, jobject thiz, jfloat speed)
{
    return (*env)->CallObjectMethod(env, thiz, class_AirStashC_PlaybackParams.method_setSpeed, speed);
}

jobject AirStashC_PlaybackParams__setSpeed__catchAll(JNIEnv *env, jobject thiz, jfloat speed)
{
    jobject ret_object = AirStashC_PlaybackParams__setSpeed(env, thiz, speed);
    if (AirStash_ExceptionCheck__catchAll(env) || !ret_object) {
        return NULL;
    }

    return ret_object;
}

jobject AirStashC_PlaybackParams__setSpeed__asGlobalRef__catchAll(JNIEnv *env, jobject thiz, jfloat speed)
{
    jobject ret_object   = NULL;
    jobject local_object = AirStashC_PlaybackParams__setSpeed__catchAll(env, thiz, speed);
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

int AirStash_loadClass__AirStashC_PlaybackParams(JNIEnv *env)
{
    int         ret                   = -1;
    const char *AIRSTASH_UNUSED(name)      = NULL;
    const char *AIRSTASH_UNUSED(sign)      = NULL;
    jclass      AIRSTASH_UNUSED(class_id)  = NULL;
    int         AIRSTASH_UNUSED(api_level) = 0;

    api_level = AirStash_GetSystemAndroidApiLevel(env);

    if (api_level < 23) {
        ALOGW("AirStashLoader: Ignore: '%s' need API %d\n", "android.media.PlaybackParams", api_level);
        goto ignore;
    }

    sign = "android/media/PlaybackParams";
    class_AirStashC_PlaybackParams.id = AirStash_FindClass__asGlobalRef__catchAll(env, sign);
    if (class_AirStashC_PlaybackParams.id == NULL)
        goto fail;

    class_id = class_AirStashC_PlaybackParams.id;
    name     = "setSpeed";
    sign     = "(F)Landroid/media/PlaybackParams;";
    class_AirStashC_PlaybackParams.method_setSpeed = AirStash_GetMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_PlaybackParams.method_setSpeed == NULL)
        goto fail;

    ALOGD("AirStashLoader: OK: '%s' loaded\n", "android.media.PlaybackParams");
ignore:
    ret = 0;
fail:
    return ret;
}
