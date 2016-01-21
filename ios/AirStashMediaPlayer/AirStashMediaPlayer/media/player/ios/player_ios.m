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


#import "player_ios.h"

#import "sdl/ios/sdl_ios.h"

#include <stdio.h>
#include <assert.h>
#include "player/ff_fferror.h"
#include "player/ff_ffplay.h"
#include "player/player_internal.h"
#include "player/pipeline/ffpipeline_ffplay.h"
#include "pipeline/ffpipeline_ios.h"

AirStashMediaPlayer *airstashmp_ios_create(int (*msg_loop)(void*))
{
    AirStashMediaPlayer *mp = airstashmp_create(msg_loop);
    if (!mp)
        goto fail;

    mp->ffplayer->vout = SDL_VoutIos_CreateForGLES2();
    if (!mp->ffplayer->vout)
        goto fail;

    mp->ffplayer->pipeline = ffpipeline_create_from_ios(mp->ffplayer);
    if (!mp->ffplayer->pipeline)
        goto fail;

    return mp;

fail:
    airstashmp_dec_ref_p(&mp);
    return NULL;
}

void airstashmp_ios_set_glview_l(AirStashMediaPlayer *mp, AirStashSDLGLView *glView)
{
    assert(mp);
    assert(mp->ffplayer);
    assert(mp->ffplayer->vout);

    SDL_VoutIos_SetGLView(mp->ffplayer->vout, glView);
}

void airstashmp_ios_set_glview(AirStashMediaPlayer *mp, AirStashSDLGLView *glView)
{
    assert(mp);
    MPTRACE("airstashmp_ios_set_view(glView=%p)\n", (void*)glView);
    pthread_mutex_lock(&mp->mutex);
    airstashmp_ios_set_glview_l(mp, glView);
    pthread_mutex_unlock(&mp->mutex);
    MPTRACE("airstashmp_ios_set_view(glView=%p)=void\n", (void*)glView);
}

bool airstashmp_ios_is_videotoolbox_open_l(AirStashMediaPlayer *mp)
{
    assert(mp);
    assert(mp->ffplayer);

    return false;
}

bool airstashmp_ios_is_videotoolbox_open(AirStashMediaPlayer *mp)
{
    assert(mp);
    MPTRACE("%s()\n", __func__);
    pthread_mutex_lock(&mp->mutex);
    bool ret = airstashmp_ios_is_videotoolbox_open_l(mp);
    pthread_mutex_unlock(&mp->mutex);
    MPTRACE("%s()=%d\n", __func__, ret ? 1 : 0);
    return ret;
}
