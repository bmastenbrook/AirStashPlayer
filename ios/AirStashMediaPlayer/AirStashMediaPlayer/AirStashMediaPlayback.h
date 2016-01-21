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
#import <UIKit/UIKit.h>

typedef NS_ENUM(NSInteger, AirStashMPMovieScalingMode) {
    AirStashMPMovieScalingModeNone,       // No scaling
    AirStashMPMovieScalingModeAspectFit,  // Uniform scale until one dimension fits
    AirStashMPMovieScalingModeAspectFill, // Uniform scale until the movie fills the visible bounds. One dimension may have clipped contents
    AirStashMPMovieScalingModeFill        // Non-uniform scale. Both render dimensions will exactly match the visible bounds
};

typedef NS_ENUM(NSInteger, AirStashMPMoviePlaybackState) {
    AirStashMPMoviePlaybackStateStopped,
    AirStashMPMoviePlaybackStatePlaying,
    AirStashMPMoviePlaybackStatePaused,
    AirStashMPMoviePlaybackStateInterrupted,
    AirStashMPMoviePlaybackStateSeekingForward,
    AirStashMPMoviePlaybackStateSeekingBackward
};

typedef NS_OPTIONS(NSUInteger, AirStashMPMovieLoadState) {
    AirStashMPMovieLoadStateUnknown        = 0,
    AirStashMPMovieLoadStatePlayable       = 1 << 0,
    AirStashMPMovieLoadStatePlaythroughOK  = 1 << 1, // Playback will be automatically started in this state when shouldAutoplay is YES
    AirStashMPMovieLoadStateStalled        = 1 << 2, // Playback will be automatically paused in this state, if started
};

typedef NS_ENUM(NSInteger, AirStashMPMovieFinishReason) {
    AirStashMPMovieFinishReasonPlaybackEnded,
    AirStashMPMovieFinishReasonPlaybackError,
    AirStashMPMovieFinishReasonUserExited
};

// -----------------------------------------------------------------------------
// Thumbnails

typedef NS_ENUM(NSInteger, AirStashMPMovieTimeOption) {
    AirStashMPMovieTimeOptionNearestKeyFrame,
    AirStashMPMovieTimeOptionExact
};

@protocol AirStashMediaPlayback;

#pragma mark AirStashMediaPlayback

@protocol AirStashMediaPlayback <NSObject>

- (void)prepareToPlay;
- (void)play;
- (void)pause;
- (void)stop;
- (BOOL)isPlaying;
- (void)shutdown;
- (void)setPauseInBackground:(BOOL)pause;

@property(nonatomic, readonly)  UIView *view;
@property(nonatomic)            NSTimeInterval currentPlaybackTime;
@property(nonatomic, readonly)  NSTimeInterval duration;
@property(nonatomic, readonly)  NSTimeInterval playableDuration;
@property(nonatomic, readonly)  NSInteger bufferingProgress;

@property(nonatomic, readonly)  BOOL isPreparedToPlay;
@property(nonatomic, readonly)  AirStashMPMoviePlaybackState playbackState;
@property(nonatomic, readonly)  AirStashMPMovieLoadState loadState;

@property(nonatomic, readonly) int64_t numberOfBytesTransferred;

@property(nonatomic, readonly) CGSize naturalSize;
@property(nonatomic) AirStashMPMovieScalingMode scalingMode;
@property(nonatomic) BOOL shouldAutoplay;

@property (nonatomic) BOOL allowsMediaAirPlay;
@property (nonatomic) BOOL isDanmakuMediaAirPlay;
@property (nonatomic, readonly) BOOL airPlayMediaActive;

@property (nonatomic) float playbackRate;

- (UIImage *)thumbnailImageAtCurrentTime;

#pragma mark Notifications

#ifdef __cplusplus
#define AIRSTASH_EXTERN extern "C" __attribute__((visibility ("default")))
#else
#define AIRSTASH_EXTERN extern __attribute__((visibility ("default")))
#endif

// -----------------------------------------------------------------------------
//  MPMediaPlayback.h

// Posted when the prepared state changes of an object conforming to the MPMediaPlayback protocol changes.
// This supersedes MPMoviePlayerContentPreloadDidFinishNotification.
AIRSTASH_EXTERN NSString *const AirStashMPMediaPlaybackIsPreparedToPlayDidChangeNotification;

// -----------------------------------------------------------------------------
//  MPMoviePlayerController.h
//  Movie Player Notifications

// Posted when the scaling mode changes.
AIRSTASH_EXTERN NSString* const AirStashMPMoviePlayerScalingModeDidChangeNotification;

// Posted when movie playback ends or a user exits playback.
AIRSTASH_EXTERN NSString* const AirStashMPMoviePlayerPlaybackDidFinishNotification;
AIRSTASH_EXTERN NSString* const AirStashMPMoviePlayerPlaybackDidFinishReasonUserInfoKey; // NSNumber (AirStashMPMovieFinishReason)

// Posted when the playback state changes, either programatically or by the user.
AIRSTASH_EXTERN NSString* const AirStashMPMoviePlayerPlaybackStateDidChangeNotification;

// Posted when the network load state changes.
AIRSTASH_EXTERN NSString* const AirStashMPMoviePlayerLoadStateDidChangeNotification;

// Posted when the movie player begins or ends playing video via AirPlay.
AIRSTASH_EXTERN NSString* const AirStashMPMoviePlayerIsAirPlayVideoActiveDidChangeNotification;

// -----------------------------------------------------------------------------
// Movie Property Notifications

// Calling -prepareToPlay on the movie player will begin determining movie properties asynchronously.
// These notifications are posted when the associated movie property becomes available.
AIRSTASH_EXTERN NSString* const AirStashMPMovieNaturalSizeAvailableNotification;

// -----------------------------------------------------------------------------
//  Extend Notifications

AIRSTASH_EXTERN NSString *const AirStashMPMoviePlayerVideoDecoderOpenNotification;
AIRSTASH_EXTERN NSString *const AirStashMPMoviePlayerFirstVideoFrameRenderedNotification;
AIRSTASH_EXTERN NSString *const AirStashMPMoviePlayerFirstAudioFrameRenderedNotification;

@end

#pragma mark AirStashMediaUrlOpenDelegate

typedef NS_ENUM(NSInteger, AirStashMediaUrlOpenType) {
    AirStashMediaUrlOpenEvent_ConcatResolveSegment = 0x10000,
    AirStashMediaUrlOpenEvent_TcpOpen = 0x10001,
    AirStashMediaUrlOpenEvent_HttpOpen = 0x10002,
    AirStashMediaUrlOpenEvent_LiveOpen = 0x10004,
};

@interface AirStashMediaUrlOpenData: NSObject

- (id)initWithUrl:(NSString *)url
         openType:(AirStashMediaUrlOpenType)openType
     segmentIndex:(int)segmentIndex
     retryCounter:(int)retryCounter;

@property(nonatomic, readonly) AirStashMediaUrlOpenType openType;
@property(nonatomic, readonly) int segmentIndex;
@property(nonatomic, readonly) int retryCounter;

@property(nonatomic, retain) NSString *url;
@property(nonatomic) int error; // set a negative value to indicate an error has occured.
@property(nonatomic, getter=isHandled)    BOOL handled;     // auto set to YES if url changed
@property(nonatomic, getter=isUrlChanged) BOOL urlChanged;  // auto set to YES by url changed

@end

@protocol AirStashMediaUrlOpenDelegate <NSObject>

- (void)willOpenUrl:(AirStashMediaUrlOpenData*) urlOpenData;

@end
