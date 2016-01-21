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

#include "Build.h"

typedef struct AirStashC_android_os_Build__VERSION {
    jclass id;

    jfieldID field_SDK_INT;
} AirStashC_android_os_Build__VERSION;
static AirStashC_android_os_Build__VERSION class_AirStashC_android_os_Build__VERSION;

typedef struct AirStashC_android_os_Build {
    jclass id;

} AirStashC_android_os_Build;
static AirStashC_android_os_Build class_AirStashC_android_os_Build;

jint AirStashC_android_os_Build__VERSION__SDK_INT__get(JNIEnv *env)
{
    return (*env)->GetStaticIntField(env, class_AirStashC_android_os_Build__VERSION.id, class_AirStashC_android_os_Build__VERSION.field_SDK_INT);
}

jint AirStashC_android_os_Build__VERSION__SDK_INT__get__catchAll(JNIEnv *env)
{
    jint ret_value = AirStashC_android_os_Build__VERSION__SDK_INT__get(env);
    if (AirStash_ExceptionCheck__catchAll(env)) {
        return 0;
    }

    return ret_value;
}

void AirStashC_android_os_Build__VERSION__SDK_INT__set(JNIEnv *env, jint value)
{
    (*env)->SetStaticIntField(env, class_AirStashC_android_os_Build__VERSION.id, class_AirStashC_android_os_Build__VERSION.field_SDK_INT, value);
}

void AirStashC_android_os_Build__VERSION__SDK_INT__set__catchAll(JNIEnv *env, jint value)
{
    AirStashC_android_os_Build__VERSION__SDK_INT__set(env, value);
    AirStash_ExceptionCheck__catchAll(env);
}

int AirStash_loadClass__AirStashC_android_os_Build__VERSION(JNIEnv *env)
{
    int         ret                   = -1;
    const char *AIRSTASH_UNUSED(name)      = NULL;
    const char *AIRSTASH_UNUSED(sign)      = NULL;
    jclass      AIRSTASH_UNUSED(class_id)  = NULL;
    int         AIRSTASH_UNUSED(api_level) = 0;

    sign = "android/os/Build$VERSION";
    class_AirStashC_android_os_Build__VERSION.id = AirStash_FindClass__asGlobalRef__catchAll(env, sign);
    if (class_AirStashC_android_os_Build__VERSION.id == NULL)
        goto fail;

    class_id = class_AirStashC_android_os_Build__VERSION.id;
    name     = "SDK_INT";
    sign     = "I";
    class_AirStashC_android_os_Build__VERSION.field_SDK_INT = AirStash_GetStaticFieldID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_android_os_Build__VERSION.field_SDK_INT == NULL)
        goto fail;

    ALOGD("AirStashLoader: OK: '%s' loaded\n", "android.os.Build$VERSION");
    ret = 0;
fail:
    return ret;
}

int AirStash_loadClass__AirStashC_android_os_Build(JNIEnv *env)
{
    int         ret                   = -1;
    const char *AIRSTASH_UNUSED(name)      = NULL;
    const char *AIRSTASH_UNUSED(sign)      = NULL;
    jclass      AIRSTASH_UNUSED(class_id)  = NULL;
    int         AIRSTASH_UNUSED(api_level) = 0;

    sign = "android/os/Build";
    class_AirStashC_android_os_Build.id = AirStash_FindClass__asGlobalRef__catchAll(env, sign);
    if (class_AirStashC_android_os_Build.id == NULL)
        goto fail;

    ret = AirStash_loadClass__AirStashC_android_os_Build__VERSION(env);
    if (ret)
        goto fail;

    ALOGD("AirStashLoader: OK: '%s' loaded\n", "android.os.Build");
    ret = 0;
fail:
    return ret;
}
