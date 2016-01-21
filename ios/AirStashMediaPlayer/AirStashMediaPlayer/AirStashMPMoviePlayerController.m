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


#import "AirStashMPMoviePlayerController.h"
#import "AirStashAudioKit.h"

@implementation AirStashMPMoviePlayerController

@dynamic view;
@dynamic currentPlaybackTime;
@dynamic duration;
@dynamic playableDuration;
@synthesize bufferingProgress = _bufferingProgress;

@dynamic isPreparedToPlay;
@dynamic playbackState;
@dynamic loadState;

@dynamic naturalSize;
@dynamic scalingMode;
@dynamic shouldAutoplay;
@synthesize isDanmakuMediaAirPlay = _isDanmakuMediaAirPlay;

@synthesize numberOfBytesTransferred = _numberOfBytesTransferred;

- (id)initWithContentURL:(NSURL *)aUrl
{
    self = [super initWithContentURL:aUrl];
    if (self) {
        self.scalingMode = MPMovieScalingModeAspectFit;
        self.shouldAutoplay = YES;
        [self AirStash_installMovieNotificationObservers];

        [[AirStashAudioKit sharedInstance] setupAudioSession];
        
        _bufferingProgress = -1;
    }
    return self;
}

- (id)initWithContentURLString:(NSString *)aUrl
{
    NSURL *url;
    if ([aUrl rangeOfString:@"/"].location == 0) {
        //本地
        url = [NSURL fileURLWithPath:aUrl];
    }
    else {
        url = [NSURL URLWithString:aUrl];
    }
    
    self = [self initWithContentURL:url];
    if (self) {
        
    }
    return self;
}

- (void)dealloc
{
    [self AirStash_removeMovieNotificationObservers];
}

- (BOOL)isPlaying
{
    switch (self.playbackState) {
        case MPMoviePlaybackStatePlaying:
            return YES;
        default:
            return NO;
    }
}

- (void)shutdown
{
    // do nothing
}

-(int64_t)numberOfBytesTransferred
{
    NSArray *events = self.accessLog.events;
    if (events.count>0) {
        MPMovieAccessLogEvent *currentEvent = [events objectAtIndex:events.count -1];
        return currentEvent.numberOfBytesTransferred;
    }
    return 0;
}

- (UIImage *)thumbnailImageAtCurrentTime
{
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    return [super thumbnailImageAtTime:self.currentPlaybackTime timeOption:MPMovieTimeOptionExact];
#pragma clang diagnostic pop
}

-(BOOL)allowsMediaAirPlay
{
    if (!self)
        return NO;
    return [self allowsAirPlay];
}

-(void)setAllowsMediaAirPlay:(BOOL)b
{
    if (!self)
        return;
    [self setAllowsAirPlay:b];
}

-(BOOL)airPlayMediaActive
{
    if (!self)
        return NO;
    
    return self.airPlayVideoActive || self.isDanmakuMediaAirPlay;
}

-(BOOL)isDanmakuMediaAirPlay
{
    return _isDanmakuMediaAirPlay;
}

-(void)setIsDanmakuMediaAirPlay:(BOOL)isDanmakuMediaAirPlay
{
    _isDanmakuMediaAirPlay = isDanmakuMediaAirPlay;
    [[NSNotificationCenter defaultCenter] postNotificationName:AirStashMPMoviePlayerIsAirPlayVideoActiveDidChangeNotification object:nil userInfo:nil];
}

-(void)setPlaybackRate:(float)playbackRate
{
    NSLog(@"[MPMoviePlayerController setPlaybackRate] is not supported\n");
}

-(float)playbackRate
{
    return 1.0f;
}

#pragma mark Movie Notification Handlers

/* Register observers for the various movie object notifications. */
-(void)AirStash_installMovieNotificationObservers
{
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(AirStash_dispatchMPMediaPlaybackIsPreparedToPlayDidChangeNotification:)
                                                 name:MPMediaPlaybackIsPreparedToPlayDidChangeNotification
                                               object:self];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(AirStash_dispatchMPMoviePlayerLoadStateDidChangeNotification:)
                                                 name:MPMoviePlayerLoadStateDidChangeNotification
                                               object:self];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(AirStash_dispatchMPMoviePlayerPlaybackDidFinishNotification:)
                                                 name:MPMoviePlayerPlaybackDidFinishNotification
                                               object:self];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(AirStash_dispatchMPMoviePlayerPlaybackStateDidChangeNotification:)
                                                 name:MPMoviePlayerPlaybackStateDidChangeNotification
                                               object:self];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(AirStash_dispatchMoviePlayerIsAirPlayVideoActiveDidChangeNotification:)
                                                 name:MPMoviePlayerIsAirPlayVideoActiveDidChangeNotification
                                               object:self];
}

- (void)AirStash_removeMovieNotificationObservers
{
    [[NSNotificationCenter defaultCenter]removeObserver:self name:MPMediaPlaybackIsPreparedToPlayDidChangeNotification object:self];
    [[NSNotificationCenter defaultCenter]removeObserver:self name:MPMoviePlayerLoadStateDidChangeNotification object:self];
    [[NSNotificationCenter defaultCenter]removeObserver:self name:MPMoviePlayerPlaybackDidFinishNotification object:self];
    [[NSNotificationCenter defaultCenter]removeObserver:self name:MPMoviePlayerPlaybackStateDidChangeNotification object:self];
    [[NSNotificationCenter defaultCenter]removeObserver:self name:MPMoviePlayerIsAirPlayVideoActiveDidChangeNotification object:self];
}

- (void)AirStash_dispatchMPMediaPlaybackIsPreparedToPlayDidChangeNotification:(NSNotification*)notification
{
    [[NSNotificationCenter defaultCenter] postNotificationName:AirStashMPMediaPlaybackIsPreparedToPlayDidChangeNotification object:notification.object userInfo:notification.userInfo];
}

- (void)AirStash_dispatchMPMoviePlayerLoadStateDidChangeNotification:(NSNotification*)notification
{
    [[NSNotificationCenter defaultCenter] postNotificationName:AirStashMPMoviePlayerLoadStateDidChangeNotification object:notification.object userInfo:notification.userInfo];
}

- (void)AirStash_dispatchMPMoviePlayerPlaybackDidFinishNotification:(NSNotification*)notification
{
    [[NSNotificationCenter defaultCenter] postNotificationName:AirStashMPMoviePlayerPlaybackDidFinishNotification object:notification.object userInfo:notification.userInfo];
}

- (void)AirStash_dispatchMPMoviePlayerPlaybackStateDidChangeNotification:(NSNotification*)notification
{
    [[NSNotificationCenter defaultCenter] postNotificationName:AirStashMPMoviePlayerPlaybackStateDidChangeNotification object:notification.object userInfo:notification.userInfo];
}

- (void)AirStash_dispatchMoviePlayerIsAirPlayVideoActiveDidChangeNotification:(NSNotification*)notification
{
    [[NSNotificationCenter defaultCenter] postNotificationName:AirStashMPMoviePlayerIsAirPlayVideoActiveDidChangeNotification object:notification.object userInfo:notification.userInfo];
}

- (void)setPauseInBackground:(BOOL)pause
{
   
}

@end
