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

#import "AirStashAudioKit.h"

@implementation AirStashAudioKit {
    BOOL _audioSessionInitialized;
}

+ (AirStashAudioKit *)sharedInstance
{
    static AirStashAudioKit *sAudioKit = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sAudioKit = [[AirStashAudioKit alloc] init];
    });
    return sAudioKit;
}

- (void)setupAudioSession
{
    if (!_audioSessionInitialized) {
        [[NSNotificationCenter defaultCenter] addObserver: self
                                                 selector: @selector(handleInterruption:)
                                                     name: AVAudioSessionInterruptionNotification
                                                   object: [AVAudioSession sharedInstance]];
        _audioSessionInitialized = YES;
    }

    /* Set audio session to mediaplayback */
    NSError *error = nil;
    if (NO == [[AVAudioSession sharedInstance] setCategory:AVAudioSessionCategoryPlayback error:&error]) {
        NSLog(@"AirStashAudioKit: AVAudioSession.setCategory() failed: %@\n", error ? [error localizedDescription] : @"nil");
        return;
    }

    error = nil;
    if (NO == [[AVAudioSession sharedInstance] setActive:YES error:&error]) {
        NSLog(@"AirStashAudioKit: AVAudioSession.setActive(YES) failed: %@\n", error ? [error localizedDescription] : @"nil");
        return;
    }

    return ;
}

- (BOOL)setActive:(BOOL)active
{
    if (active != NO) {
        [[AVAudioSession sharedInstance] setActive:YES error:nil];
    } else {
        @try {
            [[AVAudioSession sharedInstance] setActive:NO error:nil];
        } @catch (NSException *exception) {
            NSLog(@"failed to inactive AVAudioSession\n");
        }
    }
}

- (void)handleInterruption:(NSNotification *)notification
{
    int reason = [[[notification userInfo] valueForKey:AVAudioSessionInterruptionTypeKey] intValue];
    switch (reason) {
        case AVAudioSessionInterruptionTypeBegan: {
            NSLog(@"AVAudioSessionInterruptionTypeBegan\n");
            [self setActive:NO];
            break;
        }
        case AVAudioSessionInterruptionTypeEnded: {
            NSLog(@"AVAudioSessionInterruptionTypeEnded\n");
            [self setActive:YES];
            break;
        }
    }
}

@end
