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

#ifndef FFPLAY__FF_FFPIPELINE_ANDROID_H
#define FFPLAY__FF_FFPIPELINE_ANDROID_H

#include <stdbool.h>
#include <jni.h>
#include "../player_android_def.h"
#include "../../ff_ffpipeline.h"
#include "sdl/sdl_vout.h"

typedef struct FFPlayer       FFPlayer;
typedef struct AirStashFF_Pipeline AirStashFF_Pipeline;

AirStashFF_Pipeline *ffpipeline_create_from_android(FFPlayer *ffp);

void    ffpipeline_set_vout(AirStashFF_Pipeline* pipeline, SDL_Vout *vout);

int     ffpipeline_set_surface(JNIEnv *env, AirStashFF_Pipeline* pipeline, jobject surface);

int     ffpipeline_lock_surface(AirStashFF_Pipeline* pipeline);
int     ffpipeline_unlock_surface(AirStashFF_Pipeline* pipeline);
jobject ffpipeline_get_surface_as_global_ref_l(JNIEnv *env, AirStashFF_Pipeline* pipeline);
jobject ffpipeline_get_surface_as_global_ref(JNIEnv *env, AirStashFF_Pipeline* pipeline);

bool    ffpipeline_is_surface_need_reconfigure_l(AirStashFF_Pipeline* pipeline);
void    ffpipeline_set_surface_need_reconfigure_l(AirStashFF_Pipeline* pipeline, bool need_reconfigure);

void    ffpipeline_set_mediacodec_select_callback(AirStashFF_Pipeline* pipeline, bool (*callback)(void *opaque, airstashmp_mediacodecinfo_context *mcc), void *opaque);
bool    ffpipeline_select_mediacodec_l(AirStashFF_Pipeline* pipeline, airstashmp_mediacodecinfo_context *mcc);

void    ffpipeline_set_volume(AirStashFF_Pipeline* pipeline, float left, float right);

#endif
