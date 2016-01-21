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

#include "ArrayList.h"

typedef struct AirStashC_ArrayList {
    jclass id;

    jmethodID constructor_ArrayList;
    jmethodID method_add;
} AirStashC_ArrayList;
static AirStashC_ArrayList class_AirStashC_ArrayList;

jobject AirStashC_ArrayList__ArrayList(JNIEnv *env)
{
    return (*env)->NewObject(env, class_AirStashC_ArrayList.id, class_AirStashC_ArrayList.constructor_ArrayList);
}

jobject AirStashC_ArrayList__ArrayList__catchAll(JNIEnv *env)
{
    jobject ret_object = AirStashC_ArrayList__ArrayList(env);
    if (AirStash_ExceptionCheck__catchAll(env) || !ret_object) {
        return NULL;
    }

    return ret_object;
}

jobject AirStashC_ArrayList__ArrayList__asGlobalRef__catchAll(JNIEnv *env)
{
    jobject ret_object   = NULL;
    jobject local_object = AirStashC_ArrayList__ArrayList__catchAll(env);
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

jboolean AirStashC_ArrayList__add(JNIEnv *env, jobject thiz, jobject object)
{
    return (*env)->CallBooleanMethod(env, thiz, class_AirStashC_ArrayList.method_add, object);
}

jboolean AirStashC_ArrayList__add__catchAll(JNIEnv *env, jobject thiz, jobject object)
{
    jboolean ret_value = AirStashC_ArrayList__add(env, thiz, object);
    if (AirStash_ExceptionCheck__catchAll(env)) {
        return false;
    }

    return ret_value;
}

int AirStash_loadClass__AirStashC_ArrayList(JNIEnv *env)
{
    int         ret                   = -1;
    const char *AIRSTASH_UNUSED(name)      = NULL;
    const char *AIRSTASH_UNUSED(sign)      = NULL;
    jclass      AIRSTASH_UNUSED(class_id)  = NULL;
    int         AIRSTASH_UNUSED(api_level) = 0;

    sign = "java/util/ArrayList";
    class_AirStashC_ArrayList.id = AirStash_FindClass__asGlobalRef__catchAll(env, sign);
    if (class_AirStashC_ArrayList.id == NULL)
        goto fail;

    class_id = class_AirStashC_ArrayList.id;
    name     = "<init>";
    sign     = "()V";
    class_AirStashC_ArrayList.constructor_ArrayList = AirStash_GetMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_ArrayList.constructor_ArrayList == NULL)
        goto fail;

    class_id = class_AirStashC_ArrayList.id;
    name     = "add";
    sign     = "(Ljava/lang/Object;)Z";
    class_AirStashC_ArrayList.method_add = AirStash_GetMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_ArrayList.method_add == NULL)
        goto fail;

    ALOGD("AirStashLoader: OK: '%s' loaded\n", "java.util.ArrayList");
    ret = 0;
fail:
    return ret;
}
