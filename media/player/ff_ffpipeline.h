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

#ifndef FFPLAY__FF_FFPIPELINE_H
#define FFPLAY__FF_FFPIPELINE_H

#include "sdl/sdl_class.h"
#include "sdl/sdl_mutex.h"
#include "sdl/sdl_aout.h"
#include "ff_ffpipenode.h"
#include "ff_ffplay_def.h"

typedef struct AirStashFF_Pipeline_Opaque AirStashFF_Pipeline_Opaque;
typedef struct AirStashFF_Pipeline AirStashFF_Pipeline;
struct AirStashFF_Pipeline {
    SDL_Class             *opaque_class;
    AirStashFF_Pipeline_Opaque *opaque;

    void            (*func_destroy)             (AirStashFF_Pipeline *pipeline);
    AirStashFF_Pipenode *(*func_open_video_decoder)  (AirStashFF_Pipeline *pipeline, FFPlayer *ffp);
    SDL_Aout       *(*func_open_audio_output)   (AirStashFF_Pipeline *pipeline, FFPlayer *ffp);
};

AirStashFF_Pipeline *ffpipeline_alloc(SDL_Class *opaque_class, size_t opaque_size);
void ffpipeline_free(AirStashFF_Pipeline *pipeline);
void ffpipeline_free_p(AirStashFF_Pipeline **pipeline);

AirStashFF_Pipenode *ffpipeline_open_video_decoder(AirStashFF_Pipeline *pipeline, FFPlayer *ffp);
SDL_Aout       *ffpipeline_open_audio_output(AirStashFF_Pipeline *pipeline, FFPlayer *ffp);

#endif
