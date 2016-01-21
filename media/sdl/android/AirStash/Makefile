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

AIRSTASH = ./parser/airstash

ALL_SRC = \
	c/android/media/AudioTrack.c \
	c/android/media/MediaCodec.c \
	c/android/media/MediaFormat.c \
	c/android/media/PlaybackParams.c \
	c/android/os/Build.c \
	c/android/os/Bundle.c \
	c/java/nio/Buffer.c \
	c/java/nio/ByteBuffer.c \
	c/java/util/ArrayList.c \
	c/com/airstash/player/player/AirStashMediaPlayer.c \
	c/com/airstash/player/player/misc/IMediaDataSource.c \

ALL_HEAD := $(ALL_SRC:.c=.h)

all: $(ALL_SRC)

c/%.c: java/%.java
	mkdir -p $(shell dirname $@)
	$(AIRSTASH) $< $(patsubst %.c,%.h,$@) $@

.PHONY: all clean clean-all rebuild
clean:
	rm -f $(ALL_SRC)
	rm -f $(ALL_HEAD)

clean-all: clean
	cd parser && make -f ./Makefile clean && cd -

rebuild: clean all
