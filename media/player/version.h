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

#ifndef FFPLAY__VERSION_H
#define FFPLAY__VERSION_H

#include "libavutil/version.h"

/*
 * The major version number is incremented with backward incompatible changes
 *   - e.g. removing parts of the public API, reordering public struct members, etc. 
 * The minor version number is incremented for backward compatible API changes
 * or major new features
 *   - e.g. adding a new public function or a new decoder.
 * The micro version number is incremented for smaller changes that a calling program
 * might still want to check for
 *   - e.g. changing behavior in a previously unspecified situation.
 */

#define LIBAIRSTASHPLAYER_VERSION_MAJOR  2
#define LIBAIRSTASHPLAYER_VERSION_MINOR  0
#define LIBAIRSTASHPLAYER_VERSION_MICRO  0

#define LIBAIRSTASHPLAYER_VERSION_INT    AV_VERSION_INT(LIBAIRSTASHPLAYER_VERSION_MAJOR, \
                                                   LIBAIRSTASHPLAYER_VERSION_MINOR, \
                                                   LIBAIRSTASHPLAYER_VERSION_MICRO)
#define LIBAIRSTASHPLAYER_VERSION        AV_VERSION(LIBAIRSTASHPLAYER_VERSION_MAJOR,     \
                                               LIBAIRSTASHPLAYER_VERSION_MINOR,     \
                                               LIBAIRSTASHPLAYER_VERSION_MICRO)
#define LIBAIRSTASHPLAYER_BUILD          LIBAIRSTASHPLAYER_VERSION_INT

#define LIBAIRSTASHPLAYER_IDENT          "airstashplayer " AV_STRINGIFY(LIBAIRSTASHPLAYER_VERSION)

#define AIRSTASHVERSION_GET_MAJOR(x)     ((x >> 16) & 0xFF)
#define AIRSTASHVERSION_GET_MINOR(x)     ((x >>  8) & 0xFF)
#define AIRSTASHVERSION_GET_MICRO(x)     ((x      ) & 0xFF)

#endif//FFPLAY__VERSION_H
