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


rm -R -f jartemp
mkdir jartemp
cd jartemp
mkdir lib
cd lib
mkdir arm64-v8a
mkdir armeabi
mkdir armeabi-v7a
mkdir x86
mkdir x86_64
cp -f ./../../player/player-arm64/src/main/libs/arm64-v8a/*.so ./arm64-v8a/
cp -f ./../../player/player-armv5/src/main/libs/armeabi/*.so ./armeabi/
cp -f ./../../player/player-armv7a/src/main/libs/armeabi-v7a/*.so ./armeabi-v7a/
cp -f ./../../player/player-x86/src/main/libs/x86/*.so ./x86/
cp -f ./../../player/player-x86_64/src/main/libs/x86_64/*.so ./x86_64/
cd ..
zip -r -X AirStashPlayer.jar .
if [ -z "$1" ]
then
cp -f ./AirStashPlayer.jar ..
else
cp -f ./AirStashPlayer.jar ../$1
fi
cd ..
rm -R -f jartemp