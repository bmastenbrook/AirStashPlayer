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

#include "player_android.h"

#include <assert.h>
#include "sdl/android/sdl_android.h"
#include "../ff_fferror.h"
#include "../ff_ffplay.h"
#include "../player_internal.h"
#include "../pipeline/ffpipeline_ffplay.h"
#include "pipeline/ffpipeline_android.h"

AirStashMediaPlayer *airstashmp_android_create(int(*msg_loop)(void*))
{
    AirStashMediaPlayer *mp = airstashmp_create(msg_loop);
    if (!mp)
        goto fail;

    mp->ffplayer->vout = SDL_VoutAndroid_CreateForAndroidSurface();
    if (!mp->ffplayer->vout)
        goto fail;

    mp->ffplayer->pipeline = ffpipeline_create_from_android(mp->ffplayer);
    if (!mp->ffplayer->pipeline)
        goto fail;

    ffpipeline_set_vout(mp->ffplayer->pipeline, mp->ffplayer->vout);

    return mp;

fail:
    airstashmp_dec_ref_p(&mp);
    return NULL;
}

void airstashmp_android_set_surface_l(JNIEnv *env, AirStashMediaPlayer *mp, jobject android_surface)
{
    if (!mp || !mp->ffplayer || !mp->ffplayer->vout)
        return;

    SDL_VoutAndroid_SetAndroidSurface(env, mp->ffplayer->vout, android_surface);
    ffpipeline_set_surface(env, mp->ffplayer->pipeline, android_surface);
}

void airstashmp_android_set_surface(JNIEnv *env, AirStashMediaPlayer *mp, jobject android_surface)
{
    if (!mp)
        return;

    MPTRACE("airstashmp_set_android_surface(surface=%p)", (void*)android_surface);
    pthread_mutex_lock(&mp->mutex);
    airstashmp_android_set_surface_l(env, mp, android_surface);
    pthread_mutex_unlock(&mp->mutex);
    MPTRACE("airstashmp_set_android_surface(surface=%p)=void", (void*)android_surface);
}

void airstashmp_android_set_volume(JNIEnv *env, AirStashMediaPlayer *mp, float left, float right)
{
    if (!mp)
        return;

    MPTRACE("aistashmp_android_set_volume(%f, %f)", left, right);
    pthread_mutex_lock(&mp->mutex);

    if (mp && mp->ffplayer && mp->ffplayer->pipeline) {
        ffpipeline_set_volume(mp->ffplayer->pipeline, left, right);
    }

    pthread_mutex_unlock(&mp->mutex);
    MPTRACE("airstashmp_android_set_volume(%f, %f)=void", left, right);
}

int airstashmp_android_get_audio_session_id(JNIEnv *env, AirStashMediaPlayer *mp)
{
    int audio_session_id = 0;
    if (!mp)
        return audio_session_id;

    MPTRACE("%s()", __func__);
    pthread_mutex_lock(&mp->mutex);

    if (mp && mp->ffplayer && mp->ffplayer->aout) {
        audio_session_id = SDL_AoutGetAudioSessionId(mp->ffplayer->aout);
    }

    pthread_mutex_unlock(&mp->mutex);
    MPTRACE("%s()=%d", __func__, audio_session_id);

    return audio_session_id;
}

void airstashmp_android_set_mediacodec_select_callback(AirStashMediaPlayer *mp, bool (*callback)(void *opaque, airstashmp_mediacodecinfo_context *mcc), void *opaque)
{
    if (!mp)
        return;

    MPTRACE("airstashmp_android_set_mediacodec_select_callback()");
    pthread_mutex_lock(&mp->mutex);

    if (mp && mp->ffplayer && mp->ffplayer->pipeline) {
        ffpipeline_set_mediacodec_select_callback(mp->ffplayer->pipeline, callback, opaque);
    }

    pthread_mutex_unlock(&mp->mutex);
    MPTRACE("airtashmp_android_set_mediacodec_select_callback()=void");
}
