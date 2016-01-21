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

#ifndef SDL_ANDROID__ANDROID_CODEC_ANDROID_MEDIAFORMAT_INTERNAL_H
#define SDL_ANDROID__ANDROID_CODEC_ANDROID_MEDIAFORMAT_INTERNAL_H

#include "sdl_codec_android_mediaformat.h"

inline static SDL_AMediaFormat *SDL_AMediaFormat_CreateInternal(size_t opaque_size)
{
    SDL_AMediaFormat *aformat = (SDL_AMediaFormat*) mallocz(sizeof(SDL_AMediaFormat));
    if (!aformat)
        return NULL;

    aformat->opaque = mallocz(opaque_size);
    if (!aformat->opaque) {
        free(aformat);
        return NULL;
    }

    aformat->mutex = SDL_CreateMutex();
    if (aformat->mutex == NULL) {
        free(aformat->opaque);
        free(aformat);
        return NULL;
    }

    return aformat;
}

inline static void SDL_AMediaFormat_FreeInternal(SDL_AMediaFormat *aformat)
{
    if (!aformat)
        return;

    if (aformat->mutex) {
        SDL_DestroyMutexP(&aformat->mutex);
    }

    free(aformat->opaque);
    memset(aformat, 0, sizeof(SDL_AMediaFormat));
    free(aformat);
}

#endif

