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

package com.airstash.player.sample.widget.media;

import com.airstash.player.player.IMediaPlayer;
import com.airstash.player.player.AirStashMediaPlayer;
import com.airstash.player.player.MediaPlayerProxy;
import com.airstash.player.player.TextureMediaPlayer;

public class MediaPlayerCompat {
    public static String getName(IMediaPlayer mp) {
        if (mp == null) {
            return "null";
        } else if (mp instanceof TextureMediaPlayer) {
            StringBuilder sb = new StringBuilder("TextureMediaPlayer <");
            IMediaPlayer internalMediaPlayer = ((TextureMediaPlayer) mp).getInternalMediaPlayer();
            if (internalMediaPlayer == null) {
                sb.append("null>");
            } else {
                sb.append(internalMediaPlayer.getClass().getSimpleName());
                sb.append(">");
            }
            return sb.toString();
        } else {
            return mp.getClass().getSimpleName();
        }
    }

    public static AirStashMediaPlayer getAirStashMediaPlayer(IMediaPlayer mp) {
        AirStashMediaPlayer mediaPlayer = null;
        if (mp == null) {
            return null;
        } if (mp instanceof AirStashMediaPlayer) {
            mediaPlayer = (AirStashMediaPlayer) mp;
        } else if (mp instanceof MediaPlayerProxy && ((MediaPlayerProxy) mp).getInternalMediaPlayer() instanceof AirStashMediaPlayer) {
            mediaPlayer = (AirSTashMediaPlayer) ((MediaPlayerProxy) mp).getInternalMediaPlayer();
        }
        return mediaPlayer;
    }

    public static void selectTrack(IMediaPlayer mp, int stream) {
        AirStashMediaPlayer mediaPlayer = getAirStashMediaPlayer(mp);
        if (mediaPlayer == null)
            return;
        mediaPlayer.selectTrack(stream);
    }

    public static void deselectTrack(IMediaPlayer mp, int stream) {
        AirStashMediaPlayer mediaPlayer = getAirStashMediaPlayer(mp);
        if (mediaPlayer == null)
            return;
        mediaPlayer.deselectTrack(stream);
    }

    public static int getSelectedTrack(IMediaPlayer mp, int trackType) {
        AirStashMediaPlayer mediaPlayer = getAirStashMediaPlayer(mp);
        if (mediaPlayer == null)
            return -1;
        return mediaPlayer.getSelectedTrack(trackType);
    }
}
