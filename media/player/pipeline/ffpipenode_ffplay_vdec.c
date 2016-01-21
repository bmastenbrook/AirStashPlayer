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

#include "ffpipenode_ffplay_vdec.h"
#include "../ff_ffpipenode.h"
#include "../ff_ffplay.h"

struct AirStashFF_Pipenode_Opaque {
    FFPlayer *ffp;
};

static void func_destroy(AirStashFF_Pipenode *node)
{
    // do nothing
}

static int func_run_sync(AirStashFF_Pipenode *node)
{
    AirStashFF_Pipenode_Opaque *opaque = node->opaque;

    return ffp_video_thread(opaque->ffp);
}

AirStashFF_Pipenode *ffpipenode_create_video_decoder_from_ffplay(FFPlayer *ffp)
{
    AirStashFF_Pipenode *node = ffpipenode_alloc(sizeof(AirStashFF_Pipenode_Opaque));
    if (!node)
        return node;

    AirStashFF_Pipenode_Opaque *opaque = node->opaque;
    opaque->ffp         = ffp;

    node->func_destroy  = func_destroy;
    node->func_run_sync = func_run_sync;

    ffp_set_video_codec_info(ffp, AVCODEC_MODULE_NAME, avcodec_get_name(ffp->is->viddec.avctx->codec_id));
    ffp->stat.vdec_type = FFP_PROPV_DECODER_AVCODEC;
    return node;
}
