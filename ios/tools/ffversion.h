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


#if   defined(__aarch64__)
#   include "arm64/ffversion.h"
#elif defined(__x86_64__)
#   include "x86_64/ffversion.h"
#elif defined(__arm__)

#   if   defined(__ARM_ARCH_7S__)
#       include "armv7s/ffversion.h"
#   elif defined(__ARM_ARCH_7__)
#       include "armv7/ffversion.h"
#   else
#       error Unsupport ARM architecture
#   endif

#elif defined(__i386__)
#   include "i386/ffversion.h"
#else
#   error Unsupport architecture
#endif
