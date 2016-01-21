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
# -mfloat-abi=soft is a workaround for FP register corruption on Exynos 4210
# http://www.spinics.net/lists/arm-kernel/msg368417.html
ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
LOCAL_CFLAGS += -mfloat-abi=soft
endif
#LOCAL_CFLAGS += -std=c99
LOCAL_LDLIBS += -llog -landroid 
LOCAL_CFLAGS += -D__STDC_CONSTANT_MACROS
LOCAL_CFLAGS += -Wno-unused-variable

ifeq ($(TARGET_ARCH_ABI), arm64-v8a)
LOCAL_CFLAGS += -DARCH_ABI_64
endif

ifeq ($(TARGET_ARCH_ABI), x86_64)
LOCAL_CFLAGS += -DARCH_ABI_64
endif

LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_C_INCLUDES += $(realpath $(LOCAL_PATH)/..)
LOCAL_C_INCLUDES += $(MY_APP_FFMPEG_INCLUDE_PATH)

LOCAL_SRC_FILES += AirStashMetadataRetriever.cpp
LOCAL_SRC_FILES += ffmpeg_mediametadataretriever.c
LOCAL_SRC_FILES += mediametadataretriever.cpp
LOCAL_SRC_FILES +=  ffmpeg_utils.c

LOCAL_SHARED_LIBRARIES := ffmpeg utils sdl
LDFLAGS += -Wl -soname thumbnail.so
LOCAL_MODULE := thumbnail
include $(BUILD_SHARED_LIBRARY)
