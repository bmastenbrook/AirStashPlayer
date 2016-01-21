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

#ifndef AIRSTASH__android_os_Build__H
#define AIRSTASH__android_os_Build__H

#include "sdl/android/airstash/internal/internal.h"

jint AirStashC_android_os_Build__VERSION__SDK_INT__get(JNIEnv *env);
jint AirStashC_android_os_Build__VERSION__SDK_INT__get__catchAll(JNIEnv *env);
void AirStashC_android_os_Build__VERSION__SDK_INT__set(JNIEnv *env, jint value);
void AirStashC_android_os_Build__VERSION__SDK_INT__set__catchAll(JNIEnv *env, jint value);
int AirStash_loadClass__AirStashC_android_os_Build(JNIEnv *env);

#endif//AIRSTASH__android_os_Build__H