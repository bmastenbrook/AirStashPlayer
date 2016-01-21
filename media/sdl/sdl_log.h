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

#ifndef SDL__SDL_LOG_H
#define SDL__SDL_LOG_H

#include <stdio.h>

#ifdef __ANDROID__

#include <android/log.h>

#define AIRSTASH_LOG_UNKNOWN     ANDROID_LOG_UNKNOWN
#define AIRSTASH_LOG_DEFAULT     ANDROID_LOG_DEFAULT

#define AIRSTASH_LOG_VERBOSE     ANDROID_LOG_VERBOSE
#define AIRSTASH_LOG_DEBUG       ANDROID_LOG_DEBUG
#define AIRSTASH_LOG_INFO        ANDROID_LOG_INFO
#define AIRSTASH_LOG_WARN        ANDROID_LOG_WARN
#define AIRSTASH_LOG_ERROR       ANDROID_LOG_ERROR
#define AIRSTASH_LOG_FATAL       ANDROID_LOG_FATAL
#define AIRSTASH_LOG_SILENT      ANDROID_LOG_SILENT

#define VLOG(level, TAG, ...)    ((void)__android_log_vprint(level, TAG, __VA_ARGS__))
#define ALOG(level, TAG, ...)    ((void)__android_log_print(level, TAG, __VA_ARGS__))

#else

#define AIRSTASH_LOG_UNKNOWN     0
#define AIRSTASH_LOG_DEFAULT     1

#define AIRSTASH_LOG_VERBOSE     2
#define AIRSTASH_LOG_DEBUG       3
#define AIRSTASH_LOG_INFO        4
#define AIRSTASH_LOG_WARN        5
#define AIRSTASH_LOG_ERROR       6
#define AIRSTASH_LOG_FATAL       7
#define AIRSTASH_LOG_SILENT      8

#define VLOG(level, TAG, ...)    ((void)vprintf(__VA_ARGS__))
#define ALOG(level, TAG, ...)    ((void)printf(__VA_ARGS__))

#endif

#define AIRSTASH_LOG_TAG "AIRSTASHMEDIA"

#define VLOGV(...)  VLOG(AIRSTASH_LOG_VERBOSE,   AIRSTASH_LOG_TAG, __VA_ARGS__)
#define VLOGD(...)  VLOG(AIRSTASH_LOG_DEBUG,     AIRSTASH_LOG_TAG, __VA_ARGS__)
#define VLOGI(...)  VLOG(AIRSTASH_LOG_INFO,      AIRSTASH_LOG_TAG, __VA_ARGS__)
#define VLOGW(...)  VLOG(AIRSTASH_LOG_WARN,      AIRSTASH_LOG_TAG, __VA_ARGS__)
#define VLOGE(...)  VLOG(AIRSTASH_LOG_ERROR,     AIRSTASH_LOG_TAG, __VA_ARGS__)

#define ALOGV(...)  ALOG(AIRSTASH_LOG_VERBOSE,   AIRSTASH_LOG_TAG, __VA_ARGS__)
#define ALOGD(...)  ALOG(AIRSTASH_LOG_DEBUG,     AIRSTASH_LOG_TAG, __VA_ARGS__)
#define ALOGI(...)  ALOG(AIRSTASH_LOG_INFO,      AIRSTASH_LOG_TAG, __VA_ARGS__)
#define ALOGW(...)  ALOG(AIRSTASH_LOG_WARN,      AIRSTASH_LOG_TAG, __VA_ARGS__)
#define ALOGE(...)  ALOG(AIRSTASH_LOG_ERROR,     AIRSTASH_LOG_TAG, __VA_ARGS__)
#define LOG_ALWAYS_FATAL(...)   do { ALOGE(__VA_ARGS__); exit(1); } while (0)

#endif
