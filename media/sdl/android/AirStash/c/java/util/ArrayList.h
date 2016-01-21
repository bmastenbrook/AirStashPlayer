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

#ifndef AIRSTASH__java_util_ArrayList__H
#define AIRSTASH__java_util_ArrayList__H

#include "sdl/android/airstash/internal/internal.h"

jobject AirStashC_ArrayList__ArrayList(JNIEnv *env);
jobject AirStashC_ArrayList__ArrayList__catchAll(JNIEnv *env);
jobject AirStashC_ArrayList__ArrayList__asGlobalRef__catchAll(JNIEnv *env);
jboolean AirStashC_ArrayList__add(JNIEnv *env, jobject thiz, jobject object);
jboolean AirStashC_ArrayList__add__catchAll(JNIEnv *env, jobject thiz, jobject object);
int AirStash_loadClass__AirStashC_ArrayList(JNIEnv *env);

#define AIRSTASH_HAVE__AirStashC_ArrayList

#endif//AIRSTASH__java_util_ArrayList__H
