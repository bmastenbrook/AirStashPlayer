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

#include "sdl_codec_android_mediacodec_dummy.h"
#include <assert.h>
#include "sdl/sdl_log.h"
#include "sdl_codec_android_mediadef.h"
#include "sdl_codec_android_mediacodec.h"
#include "sdl_codec_android_mediacodec_internal.h"

#define DMY_TRACE(...)
//#define DMY_TRACE ALOGE

static SDL_Class g_amediacodec_class = {
    .name = "AMediaCodecDummy",
};

typedef struct SDL_AMediaCodec_Opaque {
    bool request_stop;
    SDL_AMediaCodec_FakeFifo dummy_fifo;
} SDL_AMediaCodec_Opaque;

static SDL_AMediaFormat *SDL_AMediaCodecDummy_getOutputFormat(SDL_AMediaCodec *thiz)
{
    return NULL;
}

static sdl_amedia_status_t SDL_AMediaCodecDummy_delete(SDL_AMediaCodec* acodec)
{
    SDL_AMediaCodec_FakeFifo_destroy(&acodec->opaque->dummy_fifo);

    SDL_AMediaCodec_FreeInternal(acodec);
    return SDL_AMEDIA_OK;
}

static sdl_amedia_status_t SDL_AMediaCodecDummy_configure_surface(
    JNIEnv*env,
    SDL_AMediaCodec* acodec,
    const SDL_AMediaFormat* aformat,
    jobject android_surface,
    SDL_AMediaCrypto *crypto,
    uint32_t flags)
{
    return SDL_AMEDIA_OK;
}

static sdl_amedia_status_t SDL_AMediaCodecDummy_start(SDL_AMediaCodec* acodec)
{
    acodec->opaque->request_stop = false;
    return SDL_AMEDIA_OK;
}

static sdl_amedia_status_t SDL_AMediaCodecDummy_stop(SDL_AMediaCodec* acodec)
{
    acodec->opaque->request_stop = true;
    SDL_AMediaCodec_FakeFifo_abort(&acodec->opaque->dummy_fifo);
    return SDL_AMEDIA_OK;
}

static sdl_amedia_status_t SDL_AMediaCodecDummy_flush(SDL_AMediaCodec* acodec)
{
    SDL_AMediaCodec_FakeFifo_flush(&acodec->opaque->dummy_fifo);
    return SDL_AMEDIA_OK;
}

static ssize_t SDL_AMediaCodecDummy_writeInputData(SDL_AMediaCodec* acodec, size_t idx, const uint8_t *data, size_t size)
{
    return size;
}

static ssize_t SDL_AMediaCodecDummy_dequeueInputBuffer(SDL_AMediaCodec* acodec, int64_t timeoutUs)
{
    DMY_TRACE("%s", __func__);
    return 0;
}

static sdl_amedia_status_t SDL_AMediaCodecDummy_queueInputBuffer(SDL_AMediaCodec* acodec, size_t idx, off_t offset, size_t size, uint64_t time, uint32_t flags)
{
    DMY_TRACE("%s", __func__);
    return SDL_AMediaCodec_FakeFifo_queue(&acodec->opaque->dummy_fifo, idx, offset, size, time, flags | AMEDIACODEC__BUFFER_FLAG_FAKE_FRAME);
}

static ssize_t SDL_AMediaCodecDummy_dequeueOutputBuffer(SDL_AMediaCodec* acodec, SDL_AMediaCodecBufferInfo *info, int64_t timeoutUs)
{
    DMY_TRACE("%s", __func__);
    return SDL_AMediaCodec_FakeFifo_dequeue(&acodec->opaque->dummy_fifo, info, timeoutUs);
}

static sdl_amedia_status_t SDL_AMediaCodecDummy_releaseOutputBuffer(SDL_AMediaCodec* acodec, size_t idx, bool render)
{
    DMY_TRACE("%s", __func__);
    return SDL_AMEDIA_OK;
}

static bool SDL_AMediaCodecDummy_isInputBuffersValid(SDL_AMediaCodec* acodec)
{
    return !acodec->opaque->request_stop;
}

SDL_AMediaCodec* SDL_AMediaCodecDummy_create()
{
    ALOGI("%s\n", __func__);

    SDL_AMediaCodec *acodec = SDL_AMediaCodec_CreateInternal(sizeof(SDL_AMediaCodec_Opaque));
    if (!acodec)
        return NULL;

    SDL_AMediaCodec_Opaque *opaque = acodec->opaque;
    SDL_AMediaCodec_FakeFifo_init(&opaque->dummy_fifo);

    acodec->opaque_class                = &g_amediacodec_class;
    acodec->func_delete                 = SDL_AMediaCodecDummy_delete;
    acodec->func_configure              = NULL;
    acodec->func_configure_surface      = SDL_AMediaCodecDummy_configure_surface;

    acodec->func_start                  = SDL_AMediaCodecDummy_start;
    acodec->func_stop                   = SDL_AMediaCodecDummy_stop;
    acodec->func_flush                  = SDL_AMediaCodecDummy_flush;

    acodec->func_writeInputData         = SDL_AMediaCodecDummy_writeInputData;

    acodec->func_dequeueInputBuffer     = SDL_AMediaCodecDummy_dequeueInputBuffer;
    acodec->func_queueInputBuffer       = SDL_AMediaCodecDummy_queueInputBuffer;

    acodec->func_dequeueOutputBuffer    = SDL_AMediaCodecDummy_dequeueOutputBuffer;
    acodec->func_getOutputFormat        = SDL_AMediaCodecDummy_getOutputFormat;
    acodec->func_releaseOutputBuffer    = SDL_AMediaCodecDummy_releaseOutputBuffer;

    acodec->func_isInputBuffersValid    = SDL_AMediaCodecDummy_isInputBuffersValid;

    SDL_AMediaCodec_increaseReference(acodec);
    return acodec;
}
