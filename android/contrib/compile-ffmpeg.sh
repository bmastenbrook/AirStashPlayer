#! /usr/bin/env bash
#
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

# This script is based on projects below
# https://github.com/yixia/FFmpeg-Android
# http://git.videolan.org/?p=vlc-ports/android.git;a=summary

#----------
UNI_BUILD_ROOT=`pwd`
FF_TARGET=$1
set -e
set +x

FF_ACT_ARCHS_32="armv5 armv7a x86"
FF_ACT_ARCHS_64="armv5 armv7a arm64 x86 x86_64"
FF_ACT_ARCHS_ALL=$FF_ACT_ARCHS_64

echo_archs() {
    echo "===================="
    echo "[*] check archs"
    echo "===================="
    echo "FF_ALL_ARCHS = $FF_ACT_ARCHS_ALL"
    echo "FF_ACT_ARCHS = $*"
    echo ""
}

echo_usage() {
    echo "Usage:"
    echo "  compile-ffmpeg.sh armv5|armv7a|arm64|x86|x86_64"
    echo "  compile-ffmpeg.sh all|all32"
    echo "  compile-ffmpeg.sh all64"
    echo "  compile-ffmpeg.sh clean"
    echo "  compile-ffmpeg.sh check"
    exit 1
}

echo_nextstep_help() {
    echo ""
    echo "--------------------"
    echo "[*] Finished"
    echo "--------------------"
    echo "# to continue to build AirStashPlayer, run script below,"
    echo "sh compile-player.sh "
}

#----------
case "$FF_TARGET" in
    "")
        echo_archs armv7a
        sh tools/do-compile-ffmpeg.sh armv7a
    ;;
    armv5|armv7a|arm64|x86|x86_64)
        echo_archs $FF_TARGET
        sh tools/do-compile-ffmpeg.sh $FF_TARGET
        echo_nextstep_help
    ;;
    all32)
        echo_archs $FF_ACT_ARCHS_32
        for ARCH in $FF_ACT_ARCHS_32
        do
            sh tools/do-compile-ffmpeg.sh $ARCH
        done
        echo_nextstep_help
    ;;
    all|all64)
        echo_archs $FF_ACT_ARCHS_64
        for ARCH in $FF_ACT_ARCHS_64
        do
            sh tools/do-compile-ffmpeg.sh $ARCH
        done
        echo_nextstep_help
    ;;
    clean)
        echo_archs FF_ACT_ARCHS_64
        for ARCH in $FF_ACT_ARCHS_ALL
        do
            if [ -d ffmpeg-$ARCH ]; then
                cd ffmpeg-$ARCH
                find . -type f -name '*.o' -delete
                find . -type f -name '*.a' -delete
                find . -type f -name '*.d' -delete
                rm -f config.h
                rm -f config.mak
                rm -f config.asm
                cd -
            fi
        done
        rm -rf ./build/ffmpeg-*
    ;;
    check)
        echo_archs FF_ACT_ARCHS_ALL
    ;;
    *)
        echo_usage
        exit 1
    ;;
esac
