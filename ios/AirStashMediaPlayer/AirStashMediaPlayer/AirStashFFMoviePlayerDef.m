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


#import "AirStashFFMoviePlayerDef.h"

@implementation AirStashFFMoviePlayerMessage
@end

@implementation AirStashFFMoviePlayerMessagePool{
    NSMutableArray *_array;
}

- (AirStashFFMoviePlayerMessagePool *)init
{
    self = [super init];
    if (self) {
        _array = [[NSMutableArray alloc] init];
    }
    return self;
}

- (AirStashFFMoviePlayerMessage *) obtain
{
    AirStashFFMoviePlayerMessage *msg = nil;

    @synchronized(self) {
        NSUInteger count = [_array count];
        if (count > 0) {
            msg = [_array objectAtIndex:count - 1];
            [_array removeLastObject];
        }
    }

    if (!msg)
        msg = [[AirStashFFMoviePlayerMessage alloc] init];

    return msg;
}

- (void) recycle:(AirStashFFMoviePlayerMessage *)msg
{
    if (!msg)
        return;

    @synchronized(self) {
        if ([_array count] <= 10)
            [_array addObject:msg];
    }
}

@end
