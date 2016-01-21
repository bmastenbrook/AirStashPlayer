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

#ifndef FFPLAY__CONFIG_H
#define FFPLAY__CONFIG_H

#include "libffmpeg/config.h"

// FIXME: merge filter related code and enable it
// remove these lines to enable avfilter
#ifdef CONFIG_AVFILTER
#undef CONFIG_AVFILTER
#endif
#define CONFIG_AVFILTER 0

#ifdef FFP_MERGE
#undef FFP_MERGE
#endif

#ifdef FFP_SUB
#undef FFP_SUB
#endif

#ifndef FFMPEG_LOG_TAG
#define FFMPEG_LOG_TAG "AIRSTASHFFMPEG"
#endif

#endif//FFPLAY__CONFIG_H
