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

#include "ffpipeline_ffplay.h"
#include "ffpipenode_ffplay_vdec.h"
#include "../ff_ffplay.h"

static SDL_Class g_pipeline_class = {
    .name = "ffpipeline_ffplay",
};

struct AirStashFF_Pipeline_Opaque {
    FFPlayer *ffp;
};

static void func_destroy(AirStashFF_Pipeline *pipeline)
{
    // do nothing
}

static AirStashFF_Pipenode *func_open_video_decoder(AirStashFF_Pipeline *pipeline, FFPlayer *ffp)
{
    return ffpipenode_create_video_decoder_from_ffplay(ffp);
}

static SDL_Aout *func_open_audio_output(AirStashFF_Pipeline *pipeline, FFPlayer *ffp)
{
    return NULL;
}

AirStashFF_Pipeline *ffpipeline_create_from_ffplay(FFPlayer *ffp)
{
    AirStashFF_Pipeline *pipeline = ffpipeline_alloc(&g_pipeline_class, sizeof(AirStashFF_Pipeline_Opaque));
    if (!pipeline)
        return pipeline;

    AirStashFF_Pipeline_Opaque *opaque = pipeline->opaque;
    opaque->ffp                   = ffp;

    pipeline->func_destroy            = func_destroy;
    pipeline->func_open_video_decoder = func_open_video_decoder;
    pipeline->func_open_audio_output  = func_open_audio_output;

    return pipeline;
}
