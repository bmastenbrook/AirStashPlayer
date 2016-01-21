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

#ifndef FFPLAY__FF_FFINC_H
#define FFPLAY__FF_FFINC_H

#include <stdbool.h>
#include <assert.h>
#include "libavutil/avstring.h"
#include "libavutil/time.h"
#include "libavformat/avformat.h"
#include "libavcodec/avfft.h"
#include "libswscale/swscale.h"
#include "libavutil/base64.h"
#include "libavutil/error.h"
#include "libavutil/opt.h"
#include "libavutil/version.h"
#include "libswresample/swresample.h"

#include "sdl/sdl.h"

typedef int (*airstash_inject_callback)(void *opaque, int type, void *data, size_t data_size);

#define FFP_OPT_CATEGORY_FORMAT 1
#define FFP_OPT_CATEGORY_CODEC  2
#define FFP_OPT_CATEGORY_SWS    3
#define FFP_OPT_CATEGORY_PLAYER 4
#define FFP_OPT_CATEGORY_SWR    5

#endif
