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

#include "Buffer.h"

typedef struct AirStashC_java_nio_Buffer {
    jclass id;
} AirStashC_java_nio_Buffer;
static AirStashC_java_nio_Buffer class_AirStashC_java_nio_Buffer;

int AirStash_loadClass__AirStashC_java_nio_Buffer(JNIEnv *env)
{
    int         ret                   = -1;
    const char *AIRSTASH_UNUSED(name)      = NULL;
    const char *AIRSTASH_UNUSED(sign)      = NULL;
    jclass      AIRSTASH_UNUSED(class_id)  = NULL;
    int         AIRSTASH_UNUSED(api_level) = 0;

    sign = "java/nio/Buffer";
    class_AirStashC_java_nio_Buffer.id = AirStash_FindClass__asGlobalRef__catchAll(env, sign);
    if (class_AirStashC_java_nio_Buffer.id == NULL)
        goto fail;

    ALOGD("AirStashLoader: OK: '%s' loaded\n", "java.nio.Buffer");
    ret = 0;
fail:
    return ret;
}
