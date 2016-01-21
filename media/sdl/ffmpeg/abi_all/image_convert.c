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

#include "../sdl_image_convert.h"
#if defined(__ANDROID__)
#include "libyuv.h"
#endif

int airstash_image_convert(int width, int height,
    enum AVPixelFormat dst_format, uint8_t **dst_data, int *dst_linesize,
    enum AVPixelFormat src_format, const uint8_t **src_data, const int *src_linesize)
{
#if defined(__ANDROID__)
    switch (src_format) {
        case AV_PIX_FMT_YUV420P:
        case AV_PIX_FMT_YUVJ420P: // FIXME: 9 not equal to AV_PIX_FMT_YUV420P, but a workaround
            switch (dst_format) {
            case AV_PIX_FMT_RGB565:
                return I420ToRGB565(
                    src_data[0], src_linesize[0],
                    src_data[1], src_linesize[1],
                    src_data[2], src_linesize[2],
                    dst_data[0], dst_linesize[0],
                    width, height);
            case AV_PIX_FMT_0BGR32:
                return I420ToABGR(
                    src_data[0], src_linesize[0],
                    src_data[1], src_linesize[1],
                    src_data[2], src_linesize[2],
                    dst_data[0], dst_linesize[0],
                    width, height);
            default:
                break;
            }
            break;
        default:
            break;
    }
#endif
    return -1;
}

