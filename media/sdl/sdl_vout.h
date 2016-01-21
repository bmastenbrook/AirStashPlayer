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

#ifndef SDL__SDL_VOUT_H
#define SDL__SDL_VOUT_H

#include "sdl_stdinc.h"
#include "sdl_class.h"
#include "sdl_mutex.h"
#include "sdl_video.h"
#include "ffmpeg/sdl_inc_ffmpeg.h"

typedef struct SDL_VoutOverlay_Opaque SDL_VoutOverlay_Opaque;
typedef struct SDL_VoutOverlay SDL_VoutOverlay;
struct SDL_VoutOverlay {
    int w; /**< Read-only */
    int h; /**< Read-only */
    Uint32 format; /**< Read-only */
    int planes; /**< Read-only */
    Uint16 *pitches; /**< in bytes, Read-only */
    Uint8 **pixels; /**< Read-write */

    int is_private;

    int sar_num;
    int sar_den;

    SDL_Class               *opaque_class;
    SDL_VoutOverlay_Opaque  *opaque;

    void    (*free_l)(SDL_VoutOverlay *overlay);
    int     (*lock)(SDL_VoutOverlay *overlay);
    int     (*unlock)(SDL_VoutOverlay *overlay);
    void    (*unref)(SDL_VoutOverlay *overlay);

    int     (*func_fill_frame)(SDL_VoutOverlay *overlay, const AVFrame *frame);
};

typedef struct SDL_Vout_Opaque SDL_Vout_Opaque;
typedef struct SDL_Vout SDL_Vout;
struct SDL_Vout {
    SDL_mutex *mutex;

    SDL_Class       *opaque_class;
    SDL_Vout_Opaque *opaque;
    SDL_VoutOverlay *(*create_overlay)(int width, int height, Uint32 format, SDL_Vout *vout);
    void (*free_l)(SDL_Vout *vout);
    int (*display_overlay)(SDL_Vout *vout, SDL_VoutOverlay *overlay);
};

void SDL_VoutFree(SDL_Vout *vout);
void SDL_VoutFreeP(SDL_Vout **pvout);
int SDL_VoutDisplayYUVOverlay(SDL_Vout *vout, SDL_VoutOverlay *overlay);

SDL_VoutOverlay *SDL_Vout_CreateOverlay(int width, int height, Uint32 format, SDL_Vout *vout);
int     SDL_VoutLockYUVOverlay(SDL_VoutOverlay *overlay);
int     SDL_VoutUnlockYUVOverlay(SDL_VoutOverlay *overlay);
void    SDL_VoutFreeYUVOverlay(SDL_VoutOverlay *overlay);
void    SDL_VoutUnrefYUVOverlay(SDL_VoutOverlay *overlay);
int     SDL_VoutFillFrameYUVOverlay(SDL_VoutOverlay *overlay, const AVFrame *frame);

#endif
