# This file is part of AirStashPlayer.
# Copyright (c) 2016 Wearable Inc.
#
# AirStashPlayer is based on ijkPlayer:
# Copyright (c) 2013-2014 Zhang Rui <bbcallen@gmail.com>
#
# Portions of ijkPlayer are based on kxmovie:
# Copyright (c) 2012 Konstantin Boukreev. All rights reserved.
#
# AirStashPlayer is free software: you can redistribute it and/or
# modify it under the terms of version 3 of the GNU Lesser General
# Public License as published by the Free Software Foundation.
#
# AirStashPlayer is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with AirStashPlayer.  If not, see
# <http://www.gnu.org/licenses/>.

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_CFLAGS += -std=c99
LOCAL_LDLIBS += -llog -landroid -lOpenSLES

LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_C_INCLUDES += $(realpath $(LOCAL_PATH)/..)
LOCAL_C_INCLUDES += $(MY_APP_FFMPEG_INCLUDE_PATH)
LOCAL_C_INCLUDES += $(realpath $(LOCAL_PATH)/../yuv/include)

LOCAL_SRC_FILES += sdl_aout.c
LOCAL_SRC_FILES += sdl_audio.c
LOCAL_SRC_FILES += sdl_error.c
LOCAL_SRC_FILES += sdl_mutex.c
LOCAL_SRC_FILES += sdl_stdinc.c
LOCAL_SRC_FILES += sdl_thread.c
LOCAL_SRC_FILES += sdl_timer.c
LOCAL_SRC_FILES += sdl_vout.c

LOCAL_SRC_FILES += dummy/sdl_vout_dummy.c

LOCAL_SRC_FILES += ffmpeg/sdl_vout_overlay_ffmpeg.c
LOCAL_SRC_FILES += ffmpeg/abi_all/image_convert.c

LOCAL_SRC_FILES += android/android_audiotrack.c
LOCAL_SRC_FILES += android/android_nativewindow.c
LOCAL_SRC_FILES += android/sdl_android_jni.c
LOCAL_SRC_FILES += android/sdl_aout_android_audiotrack.c
LOCAL_SRC_FILES += android/sdl_aout_android_opensles.c
LOCAL_SRC_FILES += android/sdl_codec_android_mediacodec_dummy.c
LOCAL_SRC_FILES += android/sdl_codec_android_mediacodec_internal.c
LOCAL_SRC_FILES += android/sdl_codec_android_mediacodec_java.c
LOCAL_SRC_FILES += android/sdl_codec_android_mediacodec.c
LOCAL_SRC_FILES += android/sdl_codec_android_mediadef.c
LOCAL_SRC_FILES += android/sdl_codec_android_mediaformat_java.c
LOCAL_SRC_FILES += android/sdl_codec_android_mediaformat.c
LOCAL_SRC_FILES += android/sdl_vout_android_nativewindow.c
LOCAL_SRC_FILES += android/sdl_vout_android_surface.c
LOCAL_SRC_FILES += android/sdl_vout_overlay_android_mediacodec.c

# TODO: move to individual lib
LOCAL_SRC_FILES += android/airstash/internal/allclasses.c
LOCAL_SRC_FILES += android/airstash/internal/internal.c
LOCAL_SRC_FILES += android/airstash/c/android/media/AudioTrack.c
LOCAL_SRC_FILES += android/airstash/c/android/media/AudioTrack.util.c
LOCAL_SRC_FILES += android/airstash/c/android/media/MediaCodec.c
LOCAL_SRC_FILES += android/airstash/c/android/media/MediaFormat.c
LOCAL_SRC_FILES += android/airstash/c/android/media/PlaybackParams.c
LOCAL_SRC_FILES += android/airstash/c/android/os/Build.c
LOCAL_SRC_FILES += android/airstash/c/android/os/Bundle.c
LOCAL_SRC_FILES += android/airstash/c/java/nio/Buffer.c
LOCAL_SRC_FILES += android/airstash/c/java/nio/ByteBuffer.c
LOCAL_SRC_FILES += android/airstash/c/java/nio/ByteBuffer.util.c
LOCAL_SRC_FILES += android/airstash/c/java/util/ArrayList.c
LOCAL_SRC_FILES += android/airstash/c/com/airstash/player/player/AirStashMediaPlayer.c
LOCAL_SRC_FILES += android/airstash/c/com/airstash/player/player/misc/IMediaDataSource.c

LOCAL_SHARED_LIBRARIES := ffmpeg
LOCAL_STATIC_LIBRARIES := cpufeatures yuv_static

LOCAL_MODULE := sdl
include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/cpufeatures)
