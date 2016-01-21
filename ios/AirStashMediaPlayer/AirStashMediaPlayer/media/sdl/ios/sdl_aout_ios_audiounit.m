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


#include "sdl_aout_ios_audiounit.h"

#include <stdbool.h>
#include <assert.h>
#include "sdl/sdl_inc_internal.h"
#include "sdl/sdl_thread.h"
#include "sdl/sdl_aout_internal.h"
#import "AirStashSDLAudioUnitController.h"
#import "AirStashSDLAudioQueueController.h"

struct SDL_Aout_Opaque {
    AirStashSDLAudioQueueController *aoutController;
};

static int aout_open_audio(SDL_Aout *aout, const SDL_AudioSpec *desired, SDL_AudioSpec *obtained)
{
    assert(desired);
    SDLTRACE("aout_open_audio()\n");
    SDL_Aout_Opaque *opaque = aout->opaque;

    opaque->aoutController = [[AirStashSDLAudioQueueController alloc] initWithAudioSpec:desired];
    if (!opaque->aoutController) {
        ALOGE("aout_open_audio_n: failed to new AudioTrcak()\n");
        return -1;
    }

    if (obtained)
        *obtained = opaque->aoutController.spec;

    return 0;
}

static void aout_pause_audio(SDL_Aout *aout, int pause_on)
{
    SDLTRACE("aout_pause_audio(%d)\n", pause_on);
    SDL_Aout_Opaque *opaque = aout->opaque;

    if (pause_on) {
        [opaque->aoutController pause];
    } else {
        [opaque->aoutController play];
    }
}

static void aout_flush_audio(SDL_Aout *aout)
{
    SDLTRACE("aout_flush_audio()\n");
    SDL_Aout_Opaque *opaque = aout->opaque;

    [opaque->aoutController flush];
}

static void aout_close_audio(SDL_Aout *aout)
{
    SDLTRACE("aout_close_audio()\n");
    SDL_Aout_Opaque *opaque = aout->opaque;

    [opaque->aoutController close];
}

static void aout_set_playback_rate(SDL_Aout *aout, float playbackRate)
{
    SDLTRACE("aout_close_audio()\n");
    SDL_Aout_Opaque *opaque = aout->opaque;

    [opaque->aoutController setPlaybackRate:playbackRate];
}

static void aout_free_l(SDL_Aout *aout)
{
    if (!aout)
        return;

    aout_close_audio(aout);

    SDL_Aout_Opaque *opaque = aout->opaque;
    if (opaque) {
        [opaque->aoutController release];
        opaque->aoutController = nil;
    }

    SDL_Aout_FreeInternal(aout);
}

SDL_Aout *SDL_AoutIos_CreateForAudioUnit()
{
    SDL_Aout *aout = SDL_Aout_CreateInternal(sizeof(SDL_Aout_Opaque));
    if (!aout)
        return NULL;

    // SDL_Aout_Opaque *opaque = aout->opaque;

    aout->free_l = aout_free_l;
    aout->open_audio  = aout_open_audio;
    aout->pause_audio = aout_pause_audio;
    aout->flush_audio = aout_flush_audio;
    aout->close_audio = aout_close_audio;

    aout->func_set_playback_rate = aout_set_playback_rate;

    return aout;
}
