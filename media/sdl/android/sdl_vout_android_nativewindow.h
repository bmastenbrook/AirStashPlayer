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

#ifndef SDL_ANDROID__SDL_VOUT_ANDROID_NATIVEWINDOW_H
#define SDL_ANDROID__SDL_VOUT_ANDROID_NATIVEWINDOW_H

#include "../sdl_stdinc.h"
#include "../sdl_vout.h"

typedef struct ANativeWindow   ANativeWindow;
typedef struct SDL_AMediaCodec SDL_AMediaCodec;

SDL_Vout *SDL_VoutAndroid_CreateForANativeWindow();
void SDL_VoutAndroid_SetNativeWindow(SDL_Vout *vout, ANativeWindow *native_window);
void             SDL_VoutAndroid_setAMediaCodec(SDL_Vout *vout, SDL_AMediaCodec *acodec);
SDL_AMediaCodec *SDL_VoutAndroid_peekAMediaCodec(SDL_Vout *vout);
void             SDL_VoutAndroid_invalidateAllBuffers(SDL_Vout *vout);

/*
 * MediaCodec buffer proxy
 */

typedef struct SDL_AMediaCodecBufferInfo SDL_AMediaCodecBufferInfo;
typedef struct SDL_AMediaCodecBufferProxy SDL_AMediaCodecBufferProxy;
void SDL_AMediaCodecBufferProxy_lock(SDL_AMediaCodecBufferProxy *proxy);
void SDL_AMediaCodecBufferProxy_unlock(SDL_AMediaCodecBufferProxy *proxy);

SDL_AMediaCodecBufferProxy *SDL_VoutAndroid_obtainBufferProxy(SDL_Vout *vout, int acodec_serial, int buffer_index, SDL_AMediaCodecBufferInfo *buffer_info);
int SDL_VoutAndroid_releaseBufferProxyP(SDL_Vout *vout, SDL_AMediaCodecBufferProxy **proxy, bool render);
int SDL_VoutAndroid_releaseBufferProxyP_l(SDL_Vout *vout, SDL_AMediaCodecBufferProxy **proxy, bool render);

#endif
