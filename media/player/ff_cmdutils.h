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

#ifndef FFPLAY__FF_CMDUTILS_H
#define FFPLAY__FF_CMDUTILS_H

#include "ff_ffinc.h"

void            print_error(const char *filename, int err);
AVDictionary  **setup_find_stream_info_opts(AVFormatContext *s, AVDictionary *codec_opts);
AVDictionary   *filter_codec_opts(AVDictionary *opts, enum AVCodecID codec_id,
                                  AVFormatContext *s, AVStream *st, AVCodec *codec);
/**
 * Realloc array to hold new_size elements of elem_size.
 * Calls exit() on failure.
 *
 * @param array array to reallocate
 * @param elem_size size in bytes of each element
 * @param size new element count will be written here
 * @param new_size number of elements to place in reallocated array
 * @return reallocated array
 */
void *grow_array(void *array, int elem_size, int *size, int new_size);

#define GROW_ARRAY(array, nb_elems)\
    array = grow_array(array, sizeof(*array), &nb_elems, nb_elems + 1)

double          get_rotation(AVStream *st);

#endif
