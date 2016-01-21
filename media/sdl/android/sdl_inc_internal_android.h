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

#ifndef PLAYER__SDL_INC_INTERNAL_ANDROID_H
#define PLAYER__SDL_INC_INTERNAL_ANDROID_H

#include <stdint.h>
#include <jni.h>

#include "../sdl_inc_internal.h"
#include "../sdl_misc.h"
#include "../sdl_log.h"

enum {
    HAL_PIXEL_FORMAT_RGBA_8888 = 1,
    HAL_PIXEL_FORMAT_RGBX_8888 = 2,
    HAL_PIXEL_FORMAT_RGB_888 = 3,
    HAL_PIXEL_FORMAT_RGB_565 = 4,
    HAL_PIXEL_FORMAT_BGRA_8888 = 5,
    HAL_PIXEL_FORMAT_RGBA_5551 = 6,
    HAL_PIXEL_FORMAT_RGBA_4444 = 7,

    /* 0x8 - 0xFF range unavailable */
    /* 0x100 - 0x1FF HAL implement */
    HAL_PIXEL_FORMAT_YV12 = 0x32315659, // YCrCb 4:2:0 Planar

    HAL_PIXEL_FORMAT_RAW_SENSOR = 0x20,
    HAL_PIXEL_FORMAT_BLOB = 0x21,
    HAL_PIXEL_FORMAT_IMPLEMENTATION_DEFINED = 0x22,

    /* Legacy formats (deprecated), used by ImageFormat.java */
    HAL_PIXEL_FORMAT_YCbCr_422_SP = 0x10, // NV16
    HAL_PIXEL_FORMAT_YCrCb_420_SP = 0x11, // NV21
    HAL_PIXEL_FORMAT_YCbCr_422_I = 0x14, // YUY2

};

#endif
