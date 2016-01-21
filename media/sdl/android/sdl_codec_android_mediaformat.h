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

#ifndef SDL_ANDROID__ANDROID_CODEC_ANDROID_MEDIAFORMAT_H
#define SDL_ANDROID__ANDROID_CODEC_ANDROID_MEDIAFORMAT_H

#include "../sdl_stdinc.h"
#include <stdbool.h>
#include <jni.h>
#include <sys/types.h>
#include "../sdl_mutex.h"
#include "sdl_codec_android_mediadef.h"

typedef struct SDL_AMediaFormat_Opaque      SDL_AMediaFormat_Opaque;
typedef struct SDL_AMediaFormat             SDL_AMediaFormat;
typedef struct SDL_AMediaFormat
{
    SDL_mutex *mutex;

    SDL_AMediaFormat_Opaque *opaque;

    sdl_amedia_status_t (*func_delete)(SDL_AMediaFormat *aformat);

    bool (*func_getInt32)(SDL_AMediaFormat* aformat, const char* name, int32_t *out);
    void (*func_setInt32)(SDL_AMediaFormat* aformat, const char* name, int32_t value);

    void (*func_setBuffer)(SDL_AMediaFormat* aformat, const char* name, void* data, size_t size);
} SDL_AMediaFormat;

sdl_amedia_status_t SDL_AMediaFormat_delete(SDL_AMediaFormat* aformat);
sdl_amedia_status_t SDL_AMediaFormat_deleteP(SDL_AMediaFormat** aformat);

bool SDL_AMediaFormat_getInt32(SDL_AMediaFormat* aformat, const char* name, int32_t *out);
void SDL_AMediaFormat_setInt32(SDL_AMediaFormat* aformat, const char* name, int32_t value);

void SDL_AMediaFormat_setBuffer(SDL_AMediaFormat* aformat, const char* name, void* data, size_t size);

#endif
