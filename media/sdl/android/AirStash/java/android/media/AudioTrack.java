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
package android.media;

@SimpleCClassName
@IncludeUtil
public class AudioTrack {
    public AudioTrack(int streamType, int sampleRateInHz, int channelConfig, int audioFormat, int bufferSizeInBytes, int mode);

    public static int   getMinBufferSize(int sampleRateInHz, int channelConfig, int audioFormat);
    public static float getMaxVolume();
    public static float getMinVolume();
    public static int   getNativeOutputSampleRate (int streamType);

    public void play();
    public void pause();
    public void stop();
    public void flush();
    public void release();

    public int write(byte[] audioData, int offsetInBytes, int sizeInBytes);

    public int setStereoVolume(float leftGain, float rightGain);
    public int getAudioSessionId();

    @MinApi(23)
    public PlaybackParams getPlaybackParams();
    @MinApi(23)
    void setPlaybackParams(PlaybackParams params);
}
