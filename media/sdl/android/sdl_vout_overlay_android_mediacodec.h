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

#ifndef SDL_ANDROID__SDL_VOUT_OVERLAY_ANDROID_MEDIACODEC_H
#define SDL_ANDROID__SDL_VOUT_OVERLAY_ANDROID_MEDIACODEC_H

#include "../sdl_stdinc.h"
#include "../sdl_vout.h"

typedef struct SDL_AMediaCodec           SDL_AMediaCodec;
typedef struct SDL_AMediaCodecBufferInfo SDL_AMediaCodecBufferInfo;

SDL_VoutOverlay *SDL_VoutAMediaCodec_CreateOverlay(int width, int height, Uint32 format, SDL_Vout *vout);

bool SDL_VoutOverlayAMediaCodec_isKindOf(SDL_VoutOverlay *overlay);
int  SDL_VoutOverlayAMediaCodec_releaseFrame_l(SDL_VoutOverlay *overlay, SDL_AMediaCodec *acodec, bool render);

#endif
