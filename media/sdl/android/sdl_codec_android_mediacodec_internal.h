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

#ifndef SDL_ANDROID__ANDROID_CODEC_ANDROID_MEDIACODEC_INTERNAL_H
#define SDL_ANDROID__ANDROID_CODEC_ANDROID_MEDIACODEC_INTERNAL_H

#include "sdl_codec_android_mediacodec.h"

SDL_AMediaCodec *SDL_AMediaCodec_CreateInternal(size_t opaque_size);
void             SDL_AMediaCodec_FreeInternal(SDL_AMediaCodec *acodec);

#define FAKE_BUFFER_QUEUE_SIZE 5

typedef struct SDL_AMediaCodec_FakeFrame {
    size_t index;
    SDL_AMediaCodecBufferInfo info;
} SDL_AMediaCodec_FakeFrame;

typedef struct SDL_AMediaCodec_FakeFifo {
    SDL_AMediaCodec_FakeFrame fakes[FAKE_BUFFER_QUEUE_SIZE];
    int begin;
    int end;
    int size;
    int should_abort;

    SDL_mutex *mutex;
    SDL_cond  *wakeup_enqueue_cond;
    SDL_cond  *wakeup_dequeue_cond;
} SDL_AMediaCodec_FakeFifo;

sdl_amedia_status_t SDL_AMediaCodec_FakeFifo_init(SDL_AMediaCodec_FakeFifo *fifo);
void                SDL_AMediaCodec_FakeFifo_abort(SDL_AMediaCodec_FakeFifo *fifo);
void                SDL_AMediaCodec_FakeFifo_destroy(SDL_AMediaCodec_FakeFifo *fifo);
sdl_amedia_status_t SDL_AMediaCodec_FakeFifo_queue(SDL_AMediaCodec_FakeFifo *fifo, size_t idx, off_t offset, size_t size, uint64_t time, uint32_t flags);
ssize_t             SDL_AMediaCodec_FakeFifo_dequeue(SDL_AMediaCodec_FakeFifo *fifo, SDL_AMediaCodecBufferInfo *info, int64_t timeoutUs);
void                SDL_AMediaCodec_FakeFifo_flush(SDL_AMediaCodec_FakeFifo *fifo);
int                 SDL_AMediaCodec_FakeFifo_size(SDL_AMediaCodec_FakeFifo *fifo);

#endif

