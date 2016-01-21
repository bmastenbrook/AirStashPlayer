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

#import <UIKit/UIKit.h>

//! Project version number for AirStashMediaFramework.
FOUNDATION_EXPORT double AirStashMediaFrameworkVersionNumber;

//! Project version string for AirStashMediaFramework.
FOUNDATION_EXPORT const unsigned char AirStashMediaFrameworkVersionString[];

// In this header, you should import all the public headers of your framework using statements like #import <AirStashMediaFramework/PublicHeader.h>
#import <AirStashMediaFramework/AirStashMediaPlayback.h>
#import <AirStashMediaFramework/AirStashMPMoviePlayerController.h>
#import <AirStashMediaFramework/AirStashFFOptions.h>
#import <AirStashMediaFramework/AirStashFFMoviePlayerController.h>
#import <AirStashMediaFramework/AirStashAVMoviePlayerController.h>
#import <AirStashMediaFramework/AirStashMediaModule.h>
#import <AirStashMediaFramework/AirStashMediaPlayer.h>

// backward compatible for old names
#define AirStashMediaPlaybackIsPreparedToPlayDidChangeNotification AirStashMPMediaPlaybackIsPreparedToPlayDidChangeNotification;
#define AirStashMoviePlayerLoadStateDidChangeNotification AirStashMPMoviePlayerLoadStateDidChangeNotification
#define AirStashMoviePlayerPlaybackDidFinishNotification AirStashMPMoviePlayerPlaybackDidFinishNotification
#define AirStashMoviePlayerPlaybackDidFinishReasonUserInfoKey AirStashMPMoviePlayerPlaybackDidFinishReasonUserInfoKey
#define AirStashMoviePlayerPlaybackStateDidChangeNotification AirStashMPMoviePlayerPlaybackStateDidChangeNotification
#define AirStashMoviePlayerIsAirPlayVideoActiveDidChangeNotification AirStashMPMoviePlayerIsAirPlayVideoActiveDidChangeNotification
#define AirStashMoviePlayerVideoDecoderOpenNotification AirStashMPMoviePlayerVideoDecoderOpenNotification
#define AirStashMoviePlayerFirstVideoFrameRenderedNotification AirStashMPMoviePlayerFirstVideoFrameRenderedNotification
#define AirStashMoviePlayerFirstAudioFrameRenderedNotification AirStashMPMoviePlayerFirstAudioFrameRenderedNotification

