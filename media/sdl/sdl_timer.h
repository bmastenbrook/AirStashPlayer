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

#ifndef SDL__SDL_TIMER_H
#define SDL__SDL_TIMER_H

#include "sdl_stdinc.h"

void SDL_Delay(Uint32 ms);

Uint64 SDL_GetTickHR(void);


typedef struct SDL_Profiler
{
    int64_t total_elapsed;
    int     total_counter;

    int64_t sample_elapsed;
    int     sample_counter;
    float   sample_per_seconds;
    int64_t average_elapsed;

    int64_t begin_time;

    int     max_sample;
} SDL_Profiler;

void    SDL_ProfilerReset(SDL_Profiler* profiler, int max_sample);
void    SDL_ProfilerBegin(SDL_Profiler* profiler);
int64_t SDL_ProfilerEnd(SDL_Profiler* profiler);

typedef struct SDL_SpeedSampler
{
    Uint64  samples[10];

    int     capacity;
    int     count;
    int     first_index;
    int     next_index;

    Uint64  last_log_time;
} SDL_SpeedSampler;

void  SDL_SpeedSamplerReset(SDL_SpeedSampler *sampler);
// return samples per seconds
float SDL_SpeedSamplerAdd(SDL_SpeedSampler *sampler, int enable_log, const char *log_tag);

#endif
