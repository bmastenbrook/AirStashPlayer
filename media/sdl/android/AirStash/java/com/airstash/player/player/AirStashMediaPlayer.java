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
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.airstash.player.player;

import android.os.Bundle;

@SimpleCClassName
public class AirStashMediaPlayer {
    private long mNativeMediaPlayer;
    private long mNativeMediaDataSource;

    private static void postEventFromNative(Object weakThiz, int what, int arg1, int arg2, Object obj);
    private static String onSelectCodec(Object weakThiz, String mimeType, int profile, int level);
    private static boolean onNativeInvoke(Object weakThiz, int what, Bundle args);
}
