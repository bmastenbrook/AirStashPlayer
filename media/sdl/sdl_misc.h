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

#ifndef SDL__SDL_MISC_H
#define SDL__SDL_MISC_H

#include <stdlib.h>
#include <memory.h>

#ifndef AIRSTASHMAX
#define AIRSTASHMAX(a, b)    ((a) > (b) ? (a) : (b))
#endif

#ifndef AIRSTASHMIN
#define AIRSTASHMIN(a, b)    ((a) < (b) ? (a) : (b))
#endif

#ifndef AIRSTASHALIGN
#define AIRSTASHALIGN(x, align) ((( x ) + (align) - 1) / (align) * (align))
#endif

#define AIRSTASH_CHECK_RET(condition__, retval__, ...) \
    if (!(condition__)) { \
        ALOGE(__VA_ARGS__); \
        return (retval__); \
    }

#ifndef NELEM
#define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))
#endif

inline static void *mallocz(size_t size)
{
    void *mem = malloc(size);
    if (!mem)
        return mem;

    memset(mem, 0, size);
    return mem;
}

inline static void freep(void **mem)
{
    if (mem && *mem) {
        free(*mem);
        *mem = NULL;
    }
}

#endif
