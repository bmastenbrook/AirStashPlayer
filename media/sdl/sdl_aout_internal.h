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

#ifndef SDL__SDL_AOUT_INTERNAL_H
#define SDL__SDL_AOUT_INTERNAL_H

#include "sdl_mutex.h"
#include "sdl_aout.h"

inline static SDL_Aout *SDL_Aout_CreateInternal(size_t opaque_size)
{
    SDL_Aout *aout = (SDL_Aout*) mallocz(sizeof(SDL_Aout));
    if (!aout)
        return NULL;

    aout->opaque = mallocz(opaque_size);
    if (!aout->opaque) {
        free(aout);
        return NULL;
    }

    aout->mutex = SDL_CreateMutex();
    if (aout->mutex == NULL) {
        free(aout->opaque);
        free(aout);
        return NULL;
    }

    return aout;
}

inline static void SDL_Aout_FreeInternal(SDL_Aout *aout)
{
    if (!aout)
        return;

    if (aout->mutex) {
        SDL_DestroyMutex(aout->mutex);
    }

    free(aout->opaque);
    memset(aout, 0, sizeof(SDL_Aout));
    free(aout);
}

#endif
