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

#ifndef AIRSTASH__android_media_AudioTrack__H
#define AIRSTASH__android_media_AudioTrack__H

#include "sdl/android/AirStash/internal/internal.h"

jobject AirStashC_AudioTrack__AudioTrack(JNIEnv *env, jint streamType, jint sampleRateInHz, jint channelConfig, jint audioFormat, jint bufferSizeInBytes, jint mode);
jobject AirStashC_AudioTrack__AudioTrack__catchAll(JNIEnv *env, jint streamType, jint sampleRateInHz, jint channelConfig, jint audioFormat, jint bufferSizeInBytes, jint mode);
jobject AirStashC_AudioTrack__AudioTrack__asGlobalRef__catchAll(JNIEnv *env, jint streamType, jint sampleRateInHz, jint channelConfig, jint audioFormat, jint bufferSizeInBytes, jint mode);
jint AirStashC_AudioTrack__getMinBufferSize(JNIEnv *env, jint sampleRateInHz, jint channelConfig, jint audioFormat);
jint AirStashC_AudioTrack__getMinBufferSize__catchAll(JNIEnv *env, jint sampleRateInHz, jint channelConfig, jint audioFormat);
jfloat AirStashC_AudioTrack__getMaxVolume(JNIEnv *env);
jfloat AirStashC_AudioTrack__getMaxVolume__catchAll(JNIEnv *env);
jfloat AirStashC_AudioTrack__getMinVolume(JNIEnv *env);
jfloat AirStashC_AudioTrack__getMinVolume__catchAll(JNIEnv *env);
jint AirStashC_AudioTrack__getNativeOutputSampleRate(JNIEnv *env, jint streamType);
jint AirStashC_AudioTrack__getNativeOutputSampleRate__catchAll(JNIEnv *env, jint streamType);
void AirStashC_AudioTrack__play(JNIEnv *env, jobject thiz);
void AirStashC_AudioTrack__play__catchAll(JNIEnv *env, jobject thiz);
void AirStashC_AudioTrack__pause(JNIEnv *env, jobject thiz);
void AirStashC_AudioTrack__pause__catchAll(JNIEnv *env, jobject thiz);
void AirStashC_AudioTrack__stop(JNIEnv *env, jobject thiz);
void AirStashC_AudioTrack__stop__catchAll(JNIEnv *env, jobject thiz);
void AirStashC_AudioTrack__flush(JNIEnv *env, jobject thiz);
void AirStashC_AudioTrack__flush__catchAll(JNIEnv *env, jobject thiz);
void AirStashC_AudioTrack__release(JNIEnv *env, jobject thiz);
void AirStashC_AudioTrack__release__catchAll(JNIEnv *env, jobject thiz);
jint AirStashC_AudioTrack__write(JNIEnv *env, jobject thiz, jbyteArray audioData, jint offsetInBytes, jint sizeInBytes);
jint AirStashC_AudioTrack__write__catchAll(JNIEnv *env, jobject thiz, jbyteArray audioData, jint offsetInBytes, jint sizeInBytes);
jint AirStashC_AudioTrack__setStereoVolume(JNIEnv *env, jobject thiz, jfloat leftGain, jfloat rightGain);
jint AirStashC_AudioTrack__setStereoVolume__catchAll(JNIEnv *env, jobject thiz, jfloat leftGain, jfloat rightGain);
jint AirStashC_AudioTrack__getAudioSessionId(JNIEnv *env, jobject thiz);
jint AirStashC_AudioTrack__getAudioSessionId__catchAll(JNIEnv *env, jobject thiz);
jobject AirStashC_AudioTrack__getPlaybackParams(JNIEnv *env, jobject thiz);
jobject AirStashC_AudioTrack__getPlaybackParams__catchAll(JNIEnv *env, jobject thiz);
jobject AirStashC_AudioTrack__getPlaybackParams__asGlobalRef__catchAll(JNIEnv *env, jobject thiz);
void AirStashC_AudioTrack__setPlaybackParams(JNIEnv *env, jobject thiz, jobject params);
void AirStashC_AudioTrack__setPlaybackParams__catchAll(JNIEnv *env, jobject thiz, jobject params);
int AirStash_loadClass__AirStashC_AudioTrack(JNIEnv *env);

#define AIRSTASH_HAVE__AirStashC_AudioTrack

#include "AudioTrack.util.h"

#endif//AIRSTASH__android_media_AudioTrack__H
