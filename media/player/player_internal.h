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

#ifndef AIRSTASHPLAYER_ANDROID__PLAYER_INTERNAL_H
#define AIRSTASHPLAYER_ANDROID__PLAYER_INTERNAL_H

#include <assert.h>
#include "sdl/sdl.h"
#include "ff_fferror.h"
#include "ff_ffplay.h"
#include "player.h"

struct AirStashMediaPlayer {
    volatile int ref_count;
    pthread_mutex_t mutex;
    FFPlayer *ffplayer;

    int (*msg_loop)(void*);
    SDL_Thread *msg_thread;
    SDL_Thread _msg_thread;

    int mp_state;
    char *data_source;
    void *weak_thiz;

    int restart;
    int restart_from_beginning;
    int seek_req;
    long seek_msec;
};

#endif
