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

import java.nio.ByteBuffer;
import android.view.Surface;

@SimpleCClassName
@MinApi(16)
public class MediaCodec {
    public static class BufferInfo {
        public int  flags;
        public int  offset;
        public long presentationTimeUs;
        public int  size;

        public BufferInfo();
    }

    public static MediaCodec createByCodecName(String name);

    public void configure(MediaFormat format, Surface surface, MediaCrypto crypto, int flags);

    public final MediaFormat getOutputFormat();

    public ByteBuffer[] getInputBuffers();

    public final int  dequeueInputBuffer(long timeoutUs);
    public final void queueInputBuffer(int index, int offset, int size, long presentationTimeUs, int flags);

    public final int  dequeueOutputBuffer(MediaCodec.BufferInfo info, long timeoutUs);
    public final void releaseOutputBuffer(int index, boolean render);

    public final void start();
    public final void stop();
    public final void flush();
    public final void release();
}
