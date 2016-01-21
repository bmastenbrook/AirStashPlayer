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


#include "ffpipeline_ios.h"
#include "ffpipenode_ios_videotoolbox_vdec.h"
#include "ffpipenode_ffplay_vdec.h"
#include "ff_ffplay.h"
#import "sdl/ios/sdl_aout_ios_audiounit.h"

struct AirStashFF_Pipeline_Opaque {
    FFPlayer    *ffp;
    bool         is_videotoolbox_open;
};

static void func_destroy(AirStashFF_Pipeline *pipeline)
{
}

static AirStashFF_Pipenode *func_open_video_decoder(AirStashFF_Pipeline *pipeline, FFPlayer *ffp)
{
    AirStashFF_Pipenode* node = NULL;
    AirStashFF_Pipeline_Opaque *opaque = pipeline->opaque;
    if (ffp->videotoolbox) {
        node = ffpipenode_create_video_decoder_from_ios_videotoolbox(ffp);
    }
    if (node == NULL) {
        ALOGE("vtb fail!!! switch to ffmpeg decode!!!! \n");
        node = ffpipenode_create_video_decoder_from_ffplay(ffp);
        ffp->stat.vdec_type = FFP_PROPV_DECODER_AVCODEC;
        opaque->is_videotoolbox_open = false;
    } else {
        ffp->stat.vdec_type = FFP_PROPV_DECODER_VIDEOTOOLBOX;
        opaque->is_videotoolbox_open = true;
    }
    ffp_notify_msg2(ffp, FFP_MSG_VIDEO_DECODER_OPEN, opaque->is_videotoolbox_open);
    return node;
}

static SDL_Aout *func_open_audio_output(AirStashFF_Pipeline *pipeline, FFPlayer *ffp)
{
    return SDL_AoutIos_CreateForAudioUnit();
}

static SDL_Class g_pipeline_class = {
    .name = "ffpipeline_ios",
};

AirStashFF_Pipeline *ffpipeline_create_from_ios(FFPlayer *ffp)
{
    AirStashFF_Pipeline *pipeline = ffpipeline_alloc(&g_pipeline_class, sizeof(AirStashFF_Pipeline_Opaque));
    if (!pipeline)
        return pipeline;

    AirStashFF_Pipeline_Opaque *opaque     = pipeline->opaque;
    opaque->ffp                       = ffp;
    pipeline->func_destroy            = func_destroy;
    pipeline->func_open_video_decoder = func_open_video_decoder;
    pipeline->func_open_audio_output  = func_open_audio_output;

    return pipeline;
}
