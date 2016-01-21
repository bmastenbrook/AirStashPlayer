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

#ifndef SDL__FFMPEG__SDL_VOUT_OVERLAY_FFMPEG_H
#define SDL__FFMPEG__SDL_VOUT_OVERLAY_FFMPEG_H

#include "../sdl_stdinc.h"
#include "../sdl_vout.h"
#include "sdl_inc_ffmpeg.h"

// TODO: 9 alignment to speed up memcpy when display
SDL_VoutOverlay *SDL_VoutFFmpeg_CreateOverlay(int width, int height, Uint32 format, SDL_Vout *vout);

#endif
