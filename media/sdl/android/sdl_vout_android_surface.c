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

#include "sdl_vout_android_surface.h"

#include <android/native_window_jni.h>
#include "../sdl_inc_internal.h"
#include "sdl_android_jni.h"
#include "sdl_vout_android_nativewindow.h"

SDL_Vout *SDL_VoutAndroid_CreateForAndroidSurface()
{
    return SDL_VoutAndroid_CreateForANativeWindow();
}

void SDL_VoutAndroid_SetAndroidSurface(JNIEnv *env, SDL_Vout *vout, jobject android_surface)
{
    ANativeWindow *native_window = NULL;
    if (android_surface) {
        native_window = ANativeWindow_fromSurface(env, android_surface);
        if (!native_window) {
            ALOGE("%s: ANativeWindow_fromSurface: failed\n", __func__);
            // do not return fail here;
        }
    }

    SDL_VoutAndroid_SetNativeWindow(vout, native_window);
    if (native_window)
        ANativeWindow_release(native_window);
}
