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

#include <limits.h>
#include "libavcodec/avcodec.h"
#include "sdl/sdl_log.h"

/* Inspired by libavcodec/hevc.c */
int convert_hevc_nal_units(const uint8_t *p_buf,uint32_t i_buf_size,
                           uint8_t *p_out_buf,uint32_t i_out_buf_size,
                           uint32_t *p_sps_pps_size,uint32_t *p_nal_size)
{
    int i, num_arrays;
    const uint8_t *p_end = p_buf + i_buf_size;
    uint32_t i_sps_pps_size = 0;

    if( i_buf_size <= 3 || ( !p_buf[0] && !p_buf[1] && p_buf[2] <= 1 ) )
        return -1;

    if( p_end - p_buf < 23 )
    {
        ALOGE( "Input Metadata too small" );
        return -1;
    }

    p_buf += 21;

    if( p_nal_size )
        *p_nal_size = (*p_buf & 0x03) + 1;
    p_buf++;

    num_arrays = *p_buf++;

    for( i = 0; i < num_arrays; i++ )
    {
        int type, cnt, j;

        if( p_end - p_buf < 3 )
        {
            ALOGE( "Input Metadata too small" );
            return -1;
        }
        type = *(p_buf++) & 0x3f;
        (void)(type);

        cnt = p_buf[0] << 8 | p_buf[1];
        p_buf += 2;

        for( j = 0; j < cnt; j++ )
        {
            int i_nal_size;

            if( p_end - p_buf < 2 )
            {
                ALOGE( "Input Metadata too small" );
                return -1;
            }

            i_nal_size = p_buf[0] << 8 | p_buf[1];
            p_buf += 2;

            if( i_nal_size < 0 || p_end - p_buf < i_nal_size )
            {
                ALOGE( "NAL unit size does not match Input Metadata size" );
                return -1;
            }

            if( i_sps_pps_size + 4 + i_nal_size > i_out_buf_size )
            {
                ALOGE( "Output buffer too small" );
                return -1;
            }

            p_out_buf[i_sps_pps_size++] = 0;
            p_out_buf[i_sps_pps_size++] = 0;
            p_out_buf[i_sps_pps_size++] = 0;
            p_out_buf[i_sps_pps_size++] = 1;

            memcpy(p_out_buf + i_sps_pps_size, p_buf, i_nal_size);
            p_buf += i_nal_size;

            i_sps_pps_size += i_nal_size;
        }
    }

    *p_sps_pps_size = i_sps_pps_size;

    return 0;
}
