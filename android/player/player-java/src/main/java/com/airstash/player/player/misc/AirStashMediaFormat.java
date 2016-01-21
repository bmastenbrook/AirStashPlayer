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

import android.annotation.TargetApi;
import android.os.Build;
import android.text.TextUtils;

import java.util.HashMap;
import java.util.Locale;
import java.util.Map;

import com.airstash.player.player.AirStashMediaMeta;

public class AirStashMediaFormat implements IMediaFormat {
    // Common
    public static final String KEY_AIRSTASH_CODEC_LONG_NAME_UI = "airstash-codec-long-name-ui";
    public static final String KEY_AIRSTASH_BIT_RATE_UI = "airstash-bit-rate-ui";

    // Video
    public static final String KEY_AIRSTASH_CODEC_PROFILE_LEVEL_UI = "airstash-profile-level-ui";
    public static final String KEY_AIRSTASH_CODEC_PIXEL_FORMAT_UI = "airstash-pixel-format-ui";
    public static final String KEY_AIRSTASH_RESOLUTION_UI = "airstash-resolution-ui";
    public static final String KEY_AIRSTASH_FRAME_RATE_UI = "airstash-frame-rate-ui";

    // Audio
    public static final String KEY_AIRSTASH_SAMPLE_RATE_UI = "airstash-sample-rate-ui";
    public static final String KEY_AIRSTASH_CHANNEL_UI = "airstash-channel-ui";

    // Codec
    public static final String CODEC_NAME_H264 = "h264";

    public final AirStashMediaMeta.AirStashStreamMeta mMediaFormat;

    public AiStashMediaFormat(AirStashMediaMeta.AirStashStreamMeta streamMeta) {
        mMediaFormat = streamMeta;
    }

    @TargetApi(Build.VERSION_CODES.JELLY_BEAN)
    @Override
    public int getInteger(String name) {
        if (mMediaFormat == null)
            return 0;

        return mMediaFormat.getInt(name);
    }

    @Override
    public String getString(String name) {
        if (mMediaFormat == null)
            return null;

        if (sFormatterMap.containsKey(name)) {
            Formatter formatter = sFormatterMap.get(name);
            return formatter.format(this);
        }

        return mMediaFormat.getString(name);
    }

    //-------------------------
    // Formatter
    //-------------------------

    private static abstract class Formatter {
        public String format(AirStashMediaFormat mediaFormat) {
            String value = doFormat(mediaFormat);
            if (TextUtils.isEmpty(value))
                return getDefaultString();
            return value;
        }

        protected abstract String doFormat(AirStashMediaFormat mediaFormat);

        @SuppressWarnings("SameReturnValue")
        protected String getDefaultString() {
            return "N/A";
        }
    }

    private static final Map<String, Formatter> sFormatterMap = new HashMap<String, Formatter>();

    {
        sFormatterMap.put(KEY_AIRSTASH_CODEC_LONG_NAME_UI, new Formatter() {
            @Override
            public String doFormat(AirStashMediaFormat mediaFormat) {
                return mMediaFormat.getString(AirStashMediaMeta.AIRSTASHM_KEY_CODEC_LONG_NAME);
            }
        });
        sFormatterMap.put(KEY_AIRSTASH_BIT_RATE_UI, new Formatter() {
            @Override
            protected String doFormat(AirStashMediaFormat mediaFormat) {
                int bitRate = mediaFormat.getInteger(AirStashMediaMeta.AIRSTASHM_KEY_BITRATE);
                if (bitRate <= 0) {
                    return null;
                } else if (bitRate < 1000) {
                    return String.format(Locale.US, "%d bit/s", bitRate);
                } else {
                    return String.format(Locale.US, "%d kb/s", bitRate / 1000);
                }
            }
        });
        sFormatterMap.put(KEY_AIRSTASH_CODEC_PROFILE_LEVEL_UI, new Formatter() {
            @Override
            protected String doFormat(AirStashMediaFormat mediaFormat) {
                String profile = mediaFormat.getString(AirStashMediaMeta.AIRSTASHM_KEY_CODEC_PROFILE);
                if (TextUtils.isEmpty(profile))
                    return null;

                StringBuilder sb = new StringBuilder();
                sb.append(profile);

                String codecName = mediaFormat.getString(AirStashMediaMeta.AIRSTASHM_KEY_CODEC_NAME);
                if (!TextUtils.isEmpty(codecName) && codecName.equalsIgnoreCase(CODEC_NAME_H264)) {
                    int level = mediaFormat.getInteger(AirStashMediaMeta.AIRSTASHM_KEY_CODEC_LEVEL);
                    if (level < 10)
                        return sb.toString();

                    sb.append(" Profile Level ");
                    sb.append((level / 10) % 10);
                    if ((level % 10) != 0) {
                        sb.append(".");
                        sb.append(level % 10);
                    }
                }

                return sb.toString();
            }
        });
        sFormatterMap.put(KEY_AIRSTASH_CODEC_PIXEL_FORMAT_UI, new Formatter() {
            @Override
            protected String doFormat(AirStashMediaFormat mediaFormat) {
                return mediaFormat.getString(AirStashMediaMeta.AIRSTASHM_KEY_CODEC_PIXEL_FORMAT);
            }
        });
        sFormatterMap.put(KEY_AIRSTASH_RESOLUTION_UI, new Formatter() {
            @Override
            protected String doFormat(AirStashMediaFormat mediaFormat) {
                int width = mediaFormat.getInteger(KEY_WIDTH);
                int height = mediaFormat.getInteger(KEY_HEIGHT);
                int sarNum = mediaFormat.getInteger(AirStashMediaMeta.AIRSTASHM_KEY_SAR_NUM);
                int sarDen = mediaFormat.getInteger(AirStashMediaMeta.AIRSTASHM_KEY_SAR_DEN);

                if (width <= 0 || height <= 0) {
                    return null;
                } else if (sarNum <= 0 || sarDen <= 0) {
                    return String.format(Locale.US, "%d x %d", width, height);
                } else {
                    return String.format(Locale.US, "%d x %d [SAR %d:%d]", width,
                            height, sarNum, sarDen);
                }
            }
        });
        sFormatterMap.put(KEY_AIRSTASH_FRAME_RATE_UI, new Formatter() {
            @Override
            protected String doFormat(AirStashMediaFormat mediaFormat) {
                int fpsNum = mediaFormat.getInteger(AirStashMediaMeta.AIRSTASHM_KEY_FPS_NUM);
                int fpsDen = mediaFormat.getInteger(AirStashMediaMeta.AIRSTASHM_KEY_FPS_DEN);
                if (fpsNum <= 0 || fpsDen <= 0) {
                    return null;
                } else {
                    return String.valueOf(((float) (fpsNum)) / fpsDen);
                }
            }
        });
        sFormatterMap.put(KEY_AIRSTASH_SAMPLE_RATE_UI, new Formatter() {
            @Override
            protected String doFormat(AirStashMediaFormat mediaFormat) {
                int sampleRate = mediaFormat.getInteger(AirStashMediaMeta.AIRSTASHM_KEY_SAMPLE_RATE);
                if (sampleRate <= 0) {
                    return null;
                } else {
                    return String.format(Locale.US, "%d Hz", sampleRate);
                }
            }
        });
        sFormatterMap.put(KEY_AIRSTASH_CHANNEL_UI, new Formatter() {
            @Override
            protected String doFormat(AirStashMediaFormat mediaFormat) {
                int channelLayout = mediaFormat.getInteger(AirStashMediaMeta.AIRSTASHM_KEY_CHANNEL_LAYOUT);
                if (channelLayout <= 0) {
                    return null;
                } else {
                    if (channelLayout == AirStashMediaMeta.AV_CH_LAYOUT_MONO) {
                        return "mono";
                    } else if (channelLayout == AirStashMediaMeta.AV_CH_LAYOUT_STEREO) {
                        return "stereo";
                    } else {
                        return String.format(Locale.US, "%x", channelLayout);
                    }
                }
            }
        });
    }
}
