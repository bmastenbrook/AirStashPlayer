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

#include "sdl_codec_android_mediaformat.h"
#include <assert.h>

// FIXME: release SDL_AMediaFormat
sdl_amedia_status_t SDL_AMediaFormat_delete(SDL_AMediaFormat* aformat)
{
    if (!aformat)
        return SDL_AMEDIA_OK;
    assert(aformat->func_delete);
    return aformat->func_delete(aformat);
}

sdl_amedia_status_t SDL_AMediaFormat_deleteP(SDL_AMediaFormat** aformat)
{
    if (!aformat)
        return SDL_AMEDIA_OK;
    sdl_amedia_status_t amc_ret = SDL_AMediaFormat_delete(*aformat);
    aformat = NULL;
    return amc_ret;
}

bool SDL_AMediaFormat_getInt32(SDL_AMediaFormat* aformat, const char* name, int32_t *out)
{
    assert(aformat->func_getInt32);
    return aformat->func_getInt32(aformat, name, out);
}

void SDL_AMediaFormat_setInt32(SDL_AMediaFormat* aformat, const char* name, int32_t value)
{
    assert(aformat->func_setInt32);
    aformat->func_setInt32(aformat, name, value);
}

void SDL_AMediaFormat_setBuffer(SDL_AMediaFormat* aformat, const char* name, void* data, size_t size)
{
    assert(aformat->func_setInt32);
    aformat->func_setBuffer(aformat, name, data, size);
}
