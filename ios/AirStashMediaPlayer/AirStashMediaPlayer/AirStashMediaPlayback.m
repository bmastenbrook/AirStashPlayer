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


#import "AirStashMediaPlayback.h"

NSString *const AirStashMPMediaPlaybackIsPreparedToPlayDidChangeNotification = @"AirStashMPMediaPlaybackIsPreparedToPlayDidChangeNotification";

NSString *const AirStashMPMoviePlayerPlaybackDidFinishNotification = @"AirStashMPMoviePlayerPlaybackDidFinishNotification";
NSString *const AirStashMPMoviePlayerPlaybackDidFinishReasonUserInfoKey =
    @"AirStashMPMoviePlayerPlaybackDidFinishReasonUserInfoKey";
NSString *const AirStashMPMoviePlayerPlaybackStateDidChangeNotification = @"AirStashMPMoviePlayerPlaybackStateDidChangeNotification";
NSString *const AirStashMPMoviePlayerLoadStateDidChangeNotification = @"AirStashMPMoviePlayerLoadStateDidChangeNotification";

NSString *const AirStashMPMoviePlayerIsAirPlayVideoActiveDidChangeNotification = @"AirStashMPMoviePlayerIsAirPlayVideoActiveDidChangeNotification";

NSString *const AirStashMPMovieNaturalSizeAvailableNotification = @"AirStashMPMovieNaturalSizeAvailableNotification";

NSString *const AirStashMPMoviePlayerVideoDecoderOpenNotification = @"AirStashMPMoviePlayerVideoDecoderOpenNotification";

NSString *const AirStashMPMoviePlayerFirstVideoFrameRenderedNotification = @"AirStashMPMoviePlayerFirstVideoFrameRenderedNotification";
NSString *const AirStashMPMoviePlayerFirstAudioFrameRenderedNotification = @"AirStashMPMoviePlayerFirstAudioFrameRenderedNotification";

@implementation AirStashMediaUrlOpenData {
    NSString *_url;
    BOOL _handled;
    BOOL _urlChanged;
}

- (id)initWithUrl:(NSString *)url
         openType:(AirStashMediaUrlOpenType)openType
     segmentIndex:(int)segmentIndex
     retryCounter:(int)retryCounter
{
    self = [super init];
    if (self) {
        self->_url          = url;
        self->_openType     = openType;
        self->_segmentIndex = segmentIndex;
        self->_retryCounter = retryCounter;

        self->_error        = 0;
        self->_handled      = NO;
        self->_urlChanged   = NO;
    }
    return self;
}

- (void)setHandled:(BOOL)handled
{
    _handled = handled;
}

- (BOOL)isHandled
{
    return _handled;
}

- (BOOL)isUrlChanged
{
    return _urlChanged;
}

- (NSString *)url
{
    return _url;
}

- (void)setUrl:(NSString *)url
{
    assert(url);

    _handled = YES;

    if (url == _url)
        return;

    if ([self.url compare:url]) {
        _urlChanged = YES;
        _url = url;
    }
}

@end
