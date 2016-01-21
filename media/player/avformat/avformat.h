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

#ifndef FFPLAY__AIRSTASHAVFORMAT_AIRSTASHAVFORMAT_H
#define FFPLAY__AIRSTASHAVFORMAT_AIRSTASHAVFORMAT_H

void airstashav_register_all(void);

#define AV_PKT_FLAG_DISCONTINUITY 0x0100

/**
 * Injection
 */

typedef struct AirStashAVInject_OnUrlOpenData {
    size_t  size;
    char    url[4096];      /* in, out */
    int     segment_index;  /* in, default = 0 */
    int     retry_counter;  /* in */

    int     is_handled;     /* out, default = false */
    int     is_url_changed; /* out, default = false */
} AirStashAVInject_OnUrlOpenData;

/**
 * Resolve segment url from concatdec
 *
 * @param data      AirStashAVInject_OnUrlOpenData*
 * @param data_size size of AirStashAVInject_OnUrlOpenData;
 * @return 0 if OK, AVERROR_xxx otherwise
 */
#define AIRSTASHAVINJECT_CONCAT_RESOLVE_SEGMENT  0x10000

/**
 * Protocol open event
 *
 * @param data      AirStashAVInject_OnUrlOpenData*
 * @param data_size size of AirStashAVInject_OnUrlOpenData;
 * @return 0 if OK, AVERROR_xxx otherwise
 */
#define AIRSTASHAVINJECT_ON_TCP_OPEN         0x10001
#define AIRSTASHAVINJECT_ON_HTTP_OPEN        0x10002
#define AIRSTASHAVINJECT_ON_HTTP_RETRY       0x10003
#define AIRSTASHAVINJECT_ON_LIVE_RETRY       0x10004


/**
 * Statistic
 */
typedef struct AirStashAVInject_AsyncStatistic {
    size_t  size;
    int64_t buf_backwards;
    int64_t buf_forwards;
    int64_t buf_capacity;
} AirStashAVInject_AsyncStatistic;

#define AIRSTASHAVINJECT_ASYNC_STATISTIC     0x11000

typedef int (*AirStashAVInjectCallback)(void *opaque, int message, void *data, size_t data_size);

AirStashAVInjectCallback airstashav_register_inject_callback(AirStashAVInjectCallback callback);
AirStashAVInjectCallback airstashav_get_inject_callback();

#endif
