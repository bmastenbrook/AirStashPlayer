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

#include "sdl_aout.h"
#include <stdlib.h>

int SDL_AoutOpenAudio(SDL_Aout *aout, const SDL_AudioSpec *desired, SDL_AudioSpec *obtained)
{
    if (aout && desired && aout->open_audio)
        return aout->open_audio(aout, desired, obtained);

    return -1;
}

void SDL_AoutPauseAudio(SDL_Aout *aout, int pause_on)
{
    if (aout && aout->pause_audio)
        aout->pause_audio(aout, pause_on);
}

void SDL_AoutFlushAudio(SDL_Aout *aout)
{
    if (aout && aout->flush_audio)
        aout->flush_audio(aout);
}

void SDL_AoutSetStereoVolume(SDL_Aout *aout, float left_volume, float right_volume)
{
    if (aout && aout->set_volume)
        aout->set_volume(aout, left_volume, right_volume);
}

void SDL_AoutCloseAudio(SDL_Aout *aout)
{
    if (aout && aout->close_audio)
        return aout->close_audio(aout);
}

void SDL_AoutFree(SDL_Aout *aout)
{
    if (!aout)
        return;

    if (aout->free_l)
        aout->free_l(aout);
    else
        free(aout);
}

void SDL_AoutFreeP(SDL_Aout **paout)
{
    if (!paout)
        return;

    SDL_AoutFree(*paout);
    *paout = NULL;
}

double SDL_AoutGetLatencySeconds(SDL_Aout *aout)
{
    if (!aout)
        return 0;

    if (aout->func_get_latency_seconds)
        return aout->func_get_latency_seconds(aout);

    return aout->minimal_latency_seconds;
}

void SDL_AoutSetDefaultLatencySeconds(SDL_Aout *aout, double latency)
{
    if (aout) {
        if (aout->func_set_default_latency_seconds)
            aout->func_set_default_latency_seconds(aout, latency);
        aout->minimal_latency_seconds = latency;
    }
}

void SDL_AoutSetPlaybackRate(SDL_Aout *aout, float playbackRate)
{
    if (aout) {
        if (aout->func_set_playback_rate)
            aout->func_set_playback_rate(aout, playbackRate);
    }
}

int SDL_AoutGetAudioSessionId(SDL_Aout *aout)
{
    if (aout) {
        if (aout->func_get_audio_session_id) {
            return aout->func_get_audio_session_id(aout);
        }
    }
    return 0;
}

