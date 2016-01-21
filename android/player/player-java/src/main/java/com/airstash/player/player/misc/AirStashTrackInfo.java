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
package com.airstash.player.player.misc;

import android.text.TextUtils;

import com.airstash.player.player.AirStashMediaMeta;

public class AirStashTrackInfo implements ITrackInfo {
    private int mTrackType = MEDIA_TRACK_TYPE_UNKNOWN;
    private AirStashMediaMeta.AirStashStreamMeta mStreamMeta;

    public AirStashTrackInfo(AirStashMediaMeta.AirStashStreamMeta streamMeta) {
        mStreamMeta = streamMeta;
    }

    public void setMediaMeta(AirStashMediaMeta.AirStashStreamMeta streamMeta) {
        mStreamMeta = streamMeta;
    }

    @Override
    public IMediaFormat getFormat() {
        return new AirStashMediaFormat(mStreamMeta);
    }

    @Override
    public String getLanguage() {
        if (mStreamMeta == null || TextUtils.isEmpty(mStreamMeta.mLanguage))
            return "und";

        return mStreamMeta.mLanguage;
    }

    @Override
    public int getTrackType() {
        return mTrackType;
    }

    public void setTrackType(int trackType) {
        mTrackType = trackType;
    }

    @Override
    public String toString() {
        return getClass().getSimpleName() + '{' + getInfoInline() + "}";
    }

    @Override
    public String getInfoInline() {
        StringBuilder out = new StringBuilder(128);
        switch (mTrackType) {
            case MEDIA_TRACK_TYPE_VIDEO:
                out.append("VIDEO");
                out.append(", ");
                out.append(mStreamMeta.getCodecShortNameInline());
                out.append(", ");
                out.append(mStreamMeta.getBitrateInline());
                out.append(", ");
                out.append(mStreamMeta.getResolutionInline());
                break;
            case MEDIA_TRACK_TYPE_AUDIO:
                out.append("AUDIO");
                out.append(", ");
                out.append(mStreamMeta.getCodecShortNameInline());
                out.append(", ");
                out.append(mStreamMeta.getBitrateInline());
                out.append(", ");
                out.append(mStreamMeta.getSampleRateInline());
                break;
            case MEDIA_TRACK_TYPE_TIMEDTEXT:
                out.append("TIMEDTEXT");
                break;
            case MEDIA_TRACK_TYPE_SUBTITLE:
                out.append("SUBTITLE");
                break;
            default:
                out.append("UNKNOWN");
                break;
        }
        return out.toString();
    }
}
