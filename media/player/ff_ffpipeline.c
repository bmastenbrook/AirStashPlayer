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

#include "ff_ffpipeline.h"
#include <stdlib.h>
#include <string.h>

AirStashFF_Pipeline *ffpipeline_alloc(SDL_Class *opaque_class, size_t opaque_size)
{
    AirStashFF_Pipeline *pipeline = (AirStashFF_Pipeline*) calloc(1, sizeof(AirStashFF_Pipeline));
    if (!pipeline)
        return NULL;

    pipeline->opaque_class = opaque_class;
    pipeline->opaque       = calloc(1, opaque_size);
    if (!pipeline->opaque) {
        free(pipeline);
        return NULL;
    }

    return pipeline;
}

void ffpipeline_free(AirStashFF_Pipeline *pipeline)
{
    if (!pipeline)
        return;

    if (pipeline->func_destroy) {
        pipeline->func_destroy(pipeline);
    }

    free(pipeline->opaque);
    memset(pipeline, 0, sizeof(AirStashFF_Pipeline));
    free(pipeline);
}

void ffpipeline_free_p(AirStashFF_Pipeline **pipeline)
{
    if (!pipeline)
        return;

    ffpipeline_free(*pipeline);
}

AirStashFF_Pipenode* ffpipeline_open_video_decoder(AirStashFF_Pipeline *pipeline, FFPlayer *ffp)
{
    return pipeline->func_open_video_decoder(pipeline, ffp);
}

SDL_Aout *ffpipeline_open_audio_output(AirStashFF_Pipeline *pipeline, FFPlayer *ffp)
{
    return pipeline->func_open_audio_output(pipeline, ffp);
}
