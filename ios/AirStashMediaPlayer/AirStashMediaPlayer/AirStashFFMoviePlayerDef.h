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


#import <Foundation/Foundation.h>
#include "player/ios/player_ios.h"
#include "player/meta.h"


struct AirStashSize {
    NSInteger width;
    NSInteger height;
};
typedef struct AirStashSize AirStashSize;

CG_INLINE AirStashSize
AirStashSizeMake(NSInteger width, NSInteger height)
{
    AirStashSize size;
    size.width = width;
    size.height = height;
    return size;
}



struct AirStashSampleAspectRatio {
    NSInteger numerator;
    NSInteger denominator;
};
typedef struct AirStashSampleAspectRatio AirStashSampleAspectRatio;

CG_INLINE AirStashSampleAspectRatio
AirStashSampleAspectRatioMake(NSInteger numerator, NSInteger denominator)
{
    AirStashSampleAspectRatio sampleAspectRatio;
    sampleAspectRatio.numerator = numerator;
    sampleAspectRatio.denominator = denominator;
    return sampleAspectRatio;
}



@interface AirStashFFMoviePlayerMessage : NSObject {
@public
    AVMessage _msg;
}
@end


@interface AirStashFFMoviePlayerMessagePool : NSObject

- (AirStashFFMoviePlayerMessagePool *)init;
- (AirStashFFMoviePlayerMessage *) obtain;
- (void) recycle:(AirStashFFMoviePlayerMessage *)msg;

@end
