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


#import "AirStashFFMoviePlayerController.h"

#import <UIKit/UIKit.h>
#import "AirStashFFMoviePlayerDef.h"
#import "AirStashMediaPlayback.h"
#import "AirStashMediaModule.h"
#import "AirStashAudioKit.h"
#import "NSString+AirStashMedia.h"

#include "string.h"
#include "player/version.h"
#include "player/avformat/avformat.h"

static const char *kAirStashFFRequiredFFmpegVersion = "ff2.8--airstash0.4.1.1--dev0.3.3--rc6";

@interface AirStashFFMoviePlayerController()

@property(nonatomic, readonly) NSDictionary *mediaMeta;
@property(nonatomic, readonly) NSDictionary *videoMeta;
@property(nonatomic, readonly) NSDictionary *audioMeta;

@end

@implementation AirStashFFMoviePlayerController {
    AirStashMediaPlayer *_mediaPlayer;
    AirStashSDLGLView *_glView;
    AirStashFFMoviePlayerMessagePool *_msgPool;
    NSString *_urlString;

    NSInteger _videoWidth;
    NSInteger _videoHeight;
    NSInteger _sampleAspectRatioNumerator;
    NSInteger _sampleAspectRatioDenominator;

    BOOL      _seeking;
    NSInteger _bufferingTime;
    NSInteger _bufferingPosition;

    BOOL _keepScreenOnWhilePlaying;
    BOOL _pauseInBackground;
    BOOL _isVideoToolboxOpen;
    BOOL _playingBeforeInterruption;
    
    NSMutableArray *_registeredNotifications;

    AirStashAVInject_AsyncStatistic _asyncStat;
    BOOL _shouldShowHudView;
    NSTimer *_hudTimer;
}

@synthesize view = _view;
@synthesize currentPlaybackTime;
@synthesize duration;
@synthesize playableDuration;
@synthesize bufferingProgress = _bufferingProgress;

@synthesize numberOfBytesTransferred = _numberOfBytesTransferred;

@synthesize isPreparedToPlay = _isPreparedToPlay;
@synthesize playbackState = _playbackState;
@synthesize loadState = _loadState;

@synthesize naturalSize = _naturalSize;
@synthesize scalingMode = _scalingMode;
@synthesize shouldAutoplay = _shouldAutoplay;

@synthesize mediaMeta = _mediaMeta;
@synthesize videoMeta = _videoMeta;
@synthesize audioMeta = _audioMeta;

@synthesize allowsMediaAirPlay = _allowsMediaAirPlay;
@synthesize airPlayMediaActive = _airPlayMediaActive;

@synthesize isDanmakuMediaAirPlay = _isDanmakuMediaAirPlay;

#define FFP_IO_STAT_STEP (50 * 1024)

// as an example
void AirStashFFIOStatDebugCallback(const char *url, int type, int bytes)
{
    static int64_t s_ff_io_stat_check_points = 0;
    static int64_t s_ff_io_stat_bytes = 0;
    if (!url)
        return;

    if (type != AIRSTASHMP_IO_STAT_READ)
        return;

    if (!av_strstart(url, "http:", NULL))
        return;

    s_ff_io_stat_bytes += bytes;
    if (s_ff_io_stat_bytes < s_ff_io_stat_check_points ||
        s_ff_io_stat_bytes > s_ff_io_stat_check_points + FFP_IO_STAT_STEP) {
        s_ff_io_stat_check_points = s_ff_io_stat_bytes;
        NSLog(@"io-stat: %s, +%d = %"PRId64"\n", url, bytes, s_ff_io_stat_bytes);
    }
}

void AirStashFFIOStatRegister(void (*cb)(const char *url, int type, int bytes))
{
    airstashmp_io_stat_register(cb);
}

void AirStashFFIOStatCompleteDebugCallback(const char *url,
                                      int64_t read_bytes, int64_t total_size,
                                      int64_t elpased_time, int64_t total_duration)
{
    if (!url)
        return;

    if (!av_strstart(url, "http:", NULL))
        return;

    NSLog(@"io-stat-complete: %s, %"PRId64"/%"PRId64", %"PRId64"/%"PRId64"\n",
          url, read_bytes, total_size, elpased_time, total_duration);
}

void AirStashFFIOStatCompleteRegister(void (*cb)(const char *url,
                                            int64_t read_bytes, int64_t total_size,
                                            int64_t elpased_time, int64_t total_duration))
{
    airstashmp_io_stat_complete_register(cb);
}

- (id)initWithContentURL:(NSURL *)aUrl
             withOptions:(AirStashFFOptions *)options
{
    if (aUrl == nil)
        return nil;

    // Detect if URL is file path and return proper string for it
    NSString *aUrlString = [aUrl isFileURL] ? [aUrl path] : [aUrl absoluteString];

    return [self initWithContentURLString:aUrlString
                              withOptions:options];
}

- (id)initWithContentURLString:(NSString *)aUrlString
                   withOptions:(AirStashFFOptions *)options
{
    if (aUrlString == nil)
        return nil;

    self = [super init];
    if (self) {
        airstashmp_global_init();
        airstashmp_global_set_inject_callback(airstashff_inject_callback);

        [AirStashFFMoviePlayerController checkIfFFmpegVersionMatch:NO];

        if (options == nil)
            options = [AirStashFFOptions optionsByDefault];

        // AirStashFFIOStatRegister(AirStashFFIOStatDebugCallback);
        // AirStashFFIOStatCompleteRegister(AirStashFFIOStatCompleteDebugCallback);

        // init fields
        _scalingMode = AirStashMPMovieScalingModeAspectFit;
        _shouldAutoplay = YES;
        memset(&_asyncStat, 0, sizeof(_asyncStat));

        // init media resource
        _urlString = aUrlString;
        _mediaMeta = [[NSDictionary alloc] init];

        // init player
        _mediaPlayer = airstashmp_ios_create(media_player_msg_loop);
        _msgPool = [[AirStashFFMoviePlayerMessagePool alloc] init];

        airstashmp_set_weak_thiz(_mediaPlayer, (__bridge_retained void *) self);
        airstashmp_set_inject_opaque(_mediaPlayer, (__bridge void *) self);
        airstashmp_set_option_int(_mediaPlayer, AIRSTASHMP_OPT_CATEGORY_PLAYER, "start-on-prepared", _shouldAutoplay ? 1 : 0);

        // init video sink
        _glView = [[AirStashSDLGLView alloc] initWithFrame:[[UIScreen mainScreen] bounds]
                                       useRenderQueue:options.useRenderQueue];
        _glView.shouldShowHudView = NO;
        _view   = _glView;
        [_glView setHudValue:nil forKey:@"scheme"];
        [_glView setHudValue:nil forKey:@"host"];
        [_glView setHudValue:nil forKey:@"path"];
        [_glView setHudValue:nil forKey:@"ip"];
        
        self.shouldShowHudView = options.showHudView;

        airstashmp_ios_set_glview(_mediaPlayer, _glView);
        airstashmp_set_option(_mediaPlayer, AIRSTASHMP_OPT_CATEGORY_PLAYER, "overlay-format", "fcc-i420");
#ifdef DEBUG
        [AirStashFFMoviePlayerController setLogLevel:k_AIRSTASH_LOG_DEBUG];
#else
        [AirStashFFMoviePlayerController setLogLevel:k_AIRSTASH_LOG_SILENT];
#endif
        // init audio sink
        [[AirStashAudioKit sharedInstance] setupAudioSession];

        [options applyTo:_mediaPlayer];
        _pauseInBackground = NO;

        // init extra
        _keepScreenOnWhilePlaying = YES;
        [self setScreenOn:YES];

        _registeredNotifications = [[NSMutableArray alloc] init];
        [self registerApplicationObservers];
    }
    return self;
}

- (void)setScreenOn: (BOOL)on
{
    [AirStashMediaModule sharedModule].mediaModuleIdleTimerDisabled = on;
    // [UIApplication sharedApplication].idleTimerDisabled = on;
}

- (void)dealloc
{
//    [self unregisterApplicationObservers];
}

- (void)setShouldAutoplay:(BOOL)shouldAutoplay
{
    _shouldAutoplay = shouldAutoplay;

    if (!_mediaPlayer)
        return;

    airstashmp_set_option_int(_mediaPlayer, AIRSTASHMP_OPT_CATEGORY_PLAYER, "start-on-prepared", _shouldAutoplay ? 1 : 0);
}

- (BOOL)shouldAutoplay
{
    return _shouldAutoplay;
}

- (void)prepareToPlay
{
    if (!_mediaPlayer)
        return;

    if (_urlString != nil) {
        [self setHudUrl:_urlString];
    }

    [self setScreenOn:_keepScreenOnWhilePlaying];

    airstashmp_set_data_source(_mediaPlayer, [_urlString UTF8String]);
    airstashmp_set_option(_mediaPlayer, AIRSTASHMP_OPT_CATEGORY_FORMAT, "safe", "0"); // for concat demuxer
    airstashmp_prepare_async(_mediaPlayer);
}

- (void)setHudUrl:(NSString *)urlString
{
    if ([[NSThread currentThread] isMainThread]) {
        NSRange range = [urlString rangeOfString:@"://"];
        if (range.location != NSNotFound) {
            NSString *urlFullScheme = [urlString substringToIndex:range.location];

            NSRange rangeOfLastScheme = [urlFullScheme rangeOfString:@":"
                                                             options:NSBackwardsSearch
                                                               range:NSMakeRange(0, range.location)];
            if (rangeOfLastScheme.location != NSNotFound) {
                NSString *urlExtra  = [urlString substringFromIndex:rangeOfLastScheme.location + 1];
                NSURL *url = [NSURL URLWithString:urlExtra];
                [_glView setHudValue:urlFullScheme  forKey:@"scheme"];
                [_glView setHudValue:url.host       forKey:@"host"];
                [_glView setHudValue:url.path       forKey:@"path"];
                return;
            }
        }

        NSURL *url = [NSURL URLWithString:urlString];
        [_glView setHudValue:url.scheme forKey:@"scheme"];
        [_glView setHudValue:url.host   forKey:@"host"];
        [_glView setHudValue:url.path   forKey:@"path"];
    } else {
        dispatch_async(dispatch_get_main_queue(), ^{
            [self setHudUrl:urlString];
        });
    }
}

- (void)play
{
    if (!_mediaPlayer)
        return;

    [self setScreenOn:_keepScreenOnWhilePlaying];

    [self startHudTimer];
    airstashmp_start(_mediaPlayer);
}

- (void)pause
{
    if (!_mediaPlayer)
        return;

    [self stopHudTimer];
    airstashmp_pause(_mediaPlayer);
}

- (void)stop
{
    if (!_mediaPlayer)
        return;

    [self setScreenOn:NO];

    [self stopHudTimer];
    airstashmp_stop(_mediaPlayer);
}

- (BOOL)isPlaying
{
    if (!_mediaPlayer)
        return NO;

    return airstashmp_is_playing(_mediaPlayer);
}

- (void)setPauseInBackground:(BOOL)pause
{
    _pauseInBackground = pause;
}

- (BOOL)isVideoToolboxOpen
{
    if (!_mediaPlayer)
        return NO;

    return _isVideoToolboxOpen;
}

inline static int getPlayerOption(AirStashFFOptionCategory category)
{
    int mp_category = -1;
    switch (category) {
        case kAirStashFFOptionCategoryFormat:
            mp_category = AIRSTASHMP_OPT_CATEGORY_FORMAT;
            break;
        case kAirStashFFOptionCategoryCodec:
            mp_category = AIRSTASHMP_OPT_CATEGORY_CODEC;
            break;
        case kAirStashFFOptionCategorySws:
            mp_category = AIRSTASHMP_OPT_CATEGORY_SWS;
            break;
        case kAirStashFFOptionCategoryPlayer:
            mp_category = AIRSTASHMP_OPT_CATEGORY_PLAYER;
            break;
        default:
            NSLog(@"unknown option category: %d\n", category);
    }
    return mp_category;
}

- (void)setOptionValue:(NSString *)value
                forKey:(NSString *)key
            ofCategory:(AirStashFFOptionCategory)category
{
    assert(_mediaPlayer);
    if (!_mediaPlayer)
        return;

    airstashmp_set_option(_mediaPlayer, getPlayerOption(category), [key UTF8String], [value UTF8String]);
}

- (void)setOptionIntValue:(int64_t)value
                   forKey:(NSString *)key
               ofCategory:(AirStashFFOptionCategory)category
{
    assert(_mediaPlayer);
    if (!_mediaPlayer)
        return;

    airstashmp_set_option_int(_mediaPlayer, getPlayerOption(category), [key UTF8String], value);
}

+ (void)setLogReport:(BOOL)preferLogReport
{
    airstashmp_global_set_log_report(preferLogReport ? 1 : 0);
}

+ (void)setLogLevel:(AirStashLogLevel)logLevel
{
    airstashmp_global_set_log_level(logLevel);
}

+ (BOOL)checkIfFFmpegVersionMatch:(BOOL)showAlert;
{
    const char *actualVersion = av_version_info();
    const char *expectVersion = kAirStashFFRequiredFFmpegVersion;
    if (0 == strcmp(actualVersion, expectVersion)) {
        return YES;
    } else {
        NSString *message = [NSString stringWithFormat:@"actual: %s\n expect: %s\n", actualVersion, expectVersion];
        NSLog(@"\n!!!!!!!!!!\n%@\n!!!!!!!!!!\n", message);
        if (showAlert) {
            UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"Unexpected FFmpeg version"
                                                                message:message
                                                               delegate:nil
                                                      cancelButtonTitle:@"OK"
                                                      otherButtonTitles:nil];
            [alertView show];
        }
        return NO;
    }
}

+ (BOOL)checkIfPlayerVersionMatch:(BOOL)showAlert
                            major:(unsigned int)major
                            minor:(unsigned int)minor
                            micro:(unsigned int)micro
{
    unsigned int actualVersion = airstashmp_version_int();
    if (actualVersion == AV_VERSION_INT(major, minor, micro)) {
        return YES;
    } else {
        if (showAlert) {
            NSString *message = [NSString stringWithFormat:@"actual: %s\n expect: %d.%d.%d\n",
                                 airstashmp_version_ident(), major, minor, micro];
            UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"Unexpected player version"
                                                                message:message
                                                               delegate:nil
                                                      cancelButtonTitle:@"OK"
                                                      otherButtonTitles:nil];
            [alertView show];
        }
        return NO;
    }
}

- (void)shutdown
{
    if (!_mediaPlayer)
        return;

    [self stopHudTimer];
    [self unregisterApplicationObservers];
    [self setScreenOn:NO];

    [self performSelectorInBackground:@selector(shutdownWaitStop:) withObject:self];
}

- (void)shutdownWaitStop:(AirStashFFMoviePlayerController *) mySelf
{
    if (!_mediaPlayer)
        return;

    airstashmp_stop(_mediaPlayer);
    airstashmp_shutdown(_mediaPlayer);

    [self performSelectorOnMainThread:@selector(shutdownClose:) withObject:self waitUntilDone:YES];
}

- (void)shutdownClose:(AirStashFFMoviePlayerController *) mySelf
{
    if (!_mediaPlayer)
        return;

    _segmentOpenDelegate    = nil;
    _tcpOpenDelegate        = nil;
    _httpOpenDelegate       = nil;
    _liveOpenDelegate       = nil;

    airstashmp_dec_ref_p(&_mediaPlayer);
}

- (AirStashMPMoviePlaybackState)playbackState
{
    if (!_mediaPlayer)
        return NO;

    AirStashMPMoviePlaybackState mpState = AirStashMPMoviePlaybackStateStopped;
    int state = airstashmp_get_state(_mediaPlayer);
    switch (state) {
        case MP_STATE_STOPPED:
        case MP_STATE_COMPLETED:
        case MP_STATE_ERROR:
        case MP_STATE_END:
            mpState = AirStashMPMoviePlaybackStateStopped;
            break;
        case MP_STATE_IDLE:
        case MP_STATE_INITIALIZED:
        case MP_STATE_ASYNC_PREPARING:
        case MP_STATE_PAUSED:
            mpState = AirStashMPMoviePlaybackStatePaused;
            break;
        case MP_STATE_PREPARED:
        case MP_STATE_STARTED: {
            if (_seeking)
                mpState = AirStashMPMoviePlaybackStateSeekingForward;
            else
                mpState = AirStashMPMoviePlaybackStatePlaying;
            break;
        }
    }
    // AirStashMPMoviePlaybackStatePlaying,
    // AirStashMPMoviePlaybackStatePaused,
    // AirStashMPMoviePlaybackStateStopped,
    // AirStashMPMoviePlaybackStateInterrupted,
    // AirStashMPMoviePlaybackStateSeekingForward,
    // AirStashMPMoviePlaybackStateSeekingBackward
    return mpState;
}

- (void)setCurrentPlaybackTime:(NSTimeInterval)aCurrentPlaybackTime
{
    if (!_mediaPlayer)
        return;

    _seeking = YES;
    [[NSNotificationCenter defaultCenter]
     postNotificationName:AirStashMPMoviePlayerPlaybackStateDidChangeNotification
     object:self];

    airstashmp_seek_to(_mediaPlayer, aCurrentPlaybackTime * 1000);
}

- (NSTimeInterval)currentPlaybackTime
{
    if (!_mediaPlayer)
        return 0.0f;

    NSTimeInterval ret = airstashmp_get_current_position(_mediaPlayer);
    if (isnan(ret) || isinf(ret))
        return -1;

    return ret / 1000;
}

- (NSTimeInterval)duration
{
    if (!_mediaPlayer)
        return 0.0f;

    NSTimeInterval ret = airstashmp_get_duration(_mediaPlayer);
    if (isnan(ret) || isinf(ret))
        return -1;

    return ret / 1000;
}

- (NSTimeInterval)playableDuration
{
    if (!_mediaPlayer)
        return 0.0f;

    NSTimeInterval demux_cache = ((NSTimeInterval)airstashmp_get_playable_duration(_mediaPlayer)) / 1000;

    int64_t buf_forwards = _asyncStat.buf_forwards;
    if (buf_forwards > 0) {
        int64_t bit_rate = airstashmp_get_property_int64(_mediaPlayer, FFP_PROP_INT64_BIT_RATE, 0);
        if (bit_rate > 0) {
            NSTimeInterval io_cache = ((float)buf_forwards) * 8 / bit_rate;
            return io_cache + demux_cache;
        }
    }

    return demux_cache;
}

- (CGSize)naturalSize
{
    return _naturalSize;
}

- (void)changeNaturalSize
{
    [self willChangeValueForKey:@"naturalSize"];
    if (_sampleAspectRatioNumerator > 0 && _sampleAspectRatioDenominator > 0) {
        self->_naturalSize = CGSizeMake(1.0f * _videoWidth * _sampleAspectRatioNumerator / _sampleAspectRatioDenominator, _videoHeight);
    } else {
        self->_naturalSize = CGSizeMake(_videoWidth, _videoHeight);
    }
    [self didChangeValueForKey:@"naturalSize"];

    if (self->_naturalSize.width > 0 && self->_naturalSize.height > 0) {
        [[NSNotificationCenter defaultCenter]
         postNotificationName:AirStashMPMovieNaturalSizeAvailableNotification
         object:self];
    }
}

- (void)setScalingMode: (AirStashMPMovieScalingMode) aScalingMode
{
    AirStashMPMovieScalingMode newScalingMode = aScalingMode;
    switch (aScalingMode) {
        case AirStashMPMovieScalingModeNone:
            [_view setContentMode:UIViewContentModeCenter];
            break;
        case AirStashMPMovieScalingModeAspectFit:
            [_view setContentMode:UIViewContentModeScaleAspectFit];
            break;
        case AirStashMPMovieScalingModeAspectFill:
            [_view setContentMode:UIViewContentModeScaleAspectFill];
            break;
        case AirStashMPMovieScalingModeFill:
            [_view setContentMode:UIViewContentModeScaleToFill];
            break;
        default:
            newScalingMode = _scalingMode;
    }

    _scalingMode = newScalingMode;
}

// deprecated, for MPMoviePlayerController compatiable
- (UIImage *)thumbnailImageAtTime:(NSTimeInterval)playbackTime timeOption:(AirStashMPMovieTimeOption)option
{
    return nil;
}

- (UIImage *)thumbnailImageAtCurrentTime
{
    if ([_view isKindOfClass:[AirStashSDLGLView class]]) {
        AirStashSDLGLView *glView = (AirStashSDLGLView *)_view;
        return [glView snapshot];
    }

    return nil;
}

- (CGFloat)fpsAtOutput
{
    return _glView.fps;
}

inline static NSString *formatedDurationMilli(int64_t duration) {
    if (duration >=  1000) {
        return [NSString stringWithFormat:@"%.2f sec", ((float)duration) / 1000];
    } else {
        return [NSString stringWithFormat:@"%ld msec", (long)duration];
    }
}

inline static NSString *formatedDurationBytesAndBitrate(int64_t bytes, int64_t bitRate) {
    if (bitRate <= 0) {
        return @"inf";
    }
    return formatedDurationMilli(((float)bytes) * 8 * 1000 / bitRate);
}

inline static NSString *formatedSize(int64_t bytes) {
    if (bytes >= 100 * 1000) {
        return [NSString stringWithFormat:@"%.2f MB", ((float)bytes) / 1000 / 1000];
    } else if (bytes >= 100) {
        return [NSString stringWithFormat:@"%.1f KB", ((float)bytes) / 1000];
    } else {
        return [NSString stringWithFormat:@"%ld B", (long)bytes];
    }
}

- (void)refreshHudView
{
    if (_mediaPlayer == nil)
        return;

    int64_t vdec = airstashmp_get_property_int64(_mediaPlayer, FFP_PROP_INT64_VIDEO_DECODER, FFP_PROPV_DECODER_UNKNOWN);
    float   vdps = airstashmp_get_property_float(_mediaPlayer, FFP_PROP_FLOAT_VIDEO_DECODE_FRAMES_PER_SECOND, .0f);
    float   vfps = airstashmp_get_property_float(_mediaPlayer, FFP_PROP_FLOAT_VIDEO_OUTPUT_FRAMES_PER_SECOND, .0f);

    switch (vdec) {
        case FFP_PROPV_DECODER_VIDEOTOOLBOX:
            [_glView setHudValue:@"VideoToolbox" forKey:@"vdec"];
            break;
        case FFP_PROPV_DECODER_AVCODEC:
            [_glView setHudValue:[NSString stringWithFormat:@"avcodec %d.%d.%d",
                                  LIBAVCODEC_VERSION_MAJOR,
                                  LIBAVCODEC_VERSION_MINOR,
                                  LIBAVCODEC_VERSION_MICRO]
                          forKey:@"vdec"];
            break;
        default:
            [_glView setHudValue:@"N/A" forKey:@"vdec"];
            break;
    }

    [_glView setHudValue:[NSString stringWithFormat:@"%.2f / %.2f", vdps, vfps] forKey:@"fps"];

    int64_t vcacheb = airstashmp_get_property_int64(_mediaPlayer, FFP_PROP_INT64_VIDEO_CACHED_BYTES, 0);
    int64_t acacheb = airstashmp_get_property_int64(_mediaPlayer, FFP_PROP_INT64_AUDIO_CACHED_BYTES, 0);
    int64_t vcached = airstashmp_get_property_int64(_mediaPlayer, FFP_PROP_INT64_VIDEO_CACHED_DURATION, 0);
    int64_t acached = airstashmp_get_property_int64(_mediaPlayer, FFP_PROP_INT64_AUDIO_CACHED_DURATION, 0);
    int64_t vcachep = airstashmp_get_property_int64(_mediaPlayer, FFP_PROP_INT64_VIDEO_CACHED_PACKETS, 0);
    int64_t acachep = airstashmp_get_property_int64(_mediaPlayer, FFP_PROP_INT64_AUDIO_CACHED_PACKETS, 0);
    [_glView setHudValue:[NSString stringWithFormat:@"%@, %@, %"PRId64" packets",
                          formatedDurationMilli(vcached),
                          formatedSize(vcacheb),
                          vcachep]
                  forKey:@"v-cache"];
    [_glView setHudValue:[NSString stringWithFormat:@"%@, %@, %"PRId64" packets",
                          formatedDurationMilli(acached),
                          formatedSize(acacheb),
                          acachep]
                  forKey:@"a-cache"];

    float avdelay = airstashmp_get_property_float(_mediaPlayer, FFP_PROP_FLOAT_AVDELAY, .0f);
    float avdiff  = airstashmp_get_property_float(_mediaPlayer, FFP_PROP_FLOAT_AVDIFF, .0f);
    [_glView setHudValue:[NSString stringWithFormat:@"%.3f %.3f", avdelay, -avdiff] forKey:@"delay"];

    int64_t bitRate = airstashmp_get_property_int64(_mediaPlayer, FFP_PROP_INT64_BIT_RATE, 0);
    [_glView setHudValue:[NSString stringWithFormat:@"-%@, %@",
                          formatedSize(_asyncStat.buf_backwards),
                          formatedDurationBytesAndBitrate(_asyncStat.buf_backwards, bitRate)]
                  forKey:@"async-backward"];
    [_glView setHudValue:[NSString stringWithFormat:@"+%@, %@",
                          formatedSize(_asyncStat.buf_forwards),
                          formatedDurationBytesAndBitrate(_asyncStat.buf_forwards, bitRate)]
                  forKey:@"async-forward"];
}

- (void)startHudTimer
{
    if (!_shouldShowHudView)
        return;

    if (_hudTimer != nil)
        return;

    if ([[NSThread currentThread] isMainThread]) {
        _glView.shouldShowHudView = YES;
        _hudTimer = [NSTimer scheduledTimerWithTimeInterval:.5f
                                                     target:self
                                                   selector:@selector(refreshHudView)
                                                   userInfo:nil
                                                    repeats:YES];
    } else {
        dispatch_async(dispatch_get_main_queue(), ^{
            [self startHudTimer];
        });
    }
}

- (void)stopHudTimer
{
    if (_hudTimer == nil)
        return;

    if ([[NSThread currentThread] isMainThread]) {
        _glView.shouldShowHudView = NO;
        [_hudTimer invalidate];
        _hudTimer = nil;
    } else {
        dispatch_async(dispatch_get_main_queue(), ^{
            [self stopHudTimer];
        });
    }
}

- (void)setShouldShowHudView:(BOOL)shouldShowHudView
{
    if (shouldShowHudView == _shouldShowHudView) {
        return;
    }
    _shouldShowHudView = shouldShowHudView;
    if (shouldShowHudView)
        [self startHudTimer];
    else
        [self stopHudTimer];
}

- (BOOL)shouldShowHudView
{
    return _shouldShowHudView;
}

- (void)setPlaybackRate:(float)playbackRate
{
    if (!_mediaPlayer)
        return;

    return airstashmp_set_playback_rate(_mediaPlayer, playbackRate);
}

- (float)playbackRate
{
    if (!_mediaPlayer)
        return 0.0f;

    return airstashmp_get_property_float(_mediaPlayer, FFP_PROP_FLOAT_PLAYBACK_RATE, 0.0f);
}

inline static void fillMetaInternal(NSMutableDictionary *meta, AirStashMediaMeta *rawMeta, const char *name, NSString *defaultValue)
{
    if (!meta || !rawMeta || !name)
        return;

    NSString *key = [NSString stringWithUTF8String:name];
    const char *value = airstashmeta_get_string_l(rawMeta, name);
    if (value) {
        [meta setObject:[NSString stringWithUTF8String:value] forKey:key];
    } else if (defaultValue) {
        [meta setObject:defaultValue forKey:key];
    } else {
        [meta removeObjectForKey:key];
    }
}

- (void)postEvent: (AirStashFFMoviePlayerMessage *)msg
{
    if (!msg)
        return;

    AVMessage *avmsg = &msg->_msg;
    switch (avmsg->what) {
        case FFP_MSG_FLUSH:
            break;
        case FFP_MSG_ERROR: {
            NSLog(@"FFP_MSG_ERROR: %d\n", avmsg->arg1);

            [self setScreenOn:NO];

            [[NSNotificationCenter defaultCenter]
             postNotificationName:AirStashMPMoviePlayerPlaybackStateDidChangeNotification
             object:self];

            [[NSNotificationCenter defaultCenter]
                postNotificationName:AirStashMPMoviePlayerPlaybackDidFinishNotification
                object:self
                userInfo:@{
                    AirStashMPMoviePlayerPlaybackDidFinishReasonUserInfoKey: @(AirStashMPMovieFinishReasonPlaybackError),
                    @"error": @(avmsg->arg1)}];
            break;
        }
        case FFP_MSG_PREPARED: {
            NSLog(@"FFP_MSG_PREPARED:\n");

            AirStashMediaMeta *rawMeta = airstashmp_get_meta_l(_mediaPlayer);
            if (rawMeta) {
                airstashmeta_lock(rawMeta);

                NSMutableDictionary *newMediaMeta = [[NSMutableDictionary alloc] init];

                fillMetaInternal(newMediaMeta, rawMeta, AIRSTASHM_KEY_FORMAT, nil);
                fillMetaInternal(newMediaMeta, rawMeta, AIRSTASHM_KEY_DURATION_US, nil);
                fillMetaInternal(newMediaMeta, rawMeta, AIRSTASHM_KEY_START_US, nil);
                fillMetaInternal(newMediaMeta, rawMeta, AIRSTASHM_KEY_BITRATE, nil);

                fillMetaInternal(newMediaMeta, rawMeta, AIRSTASHM_KEY_VIDEO_STREAM, nil);
                fillMetaInternal(newMediaMeta, rawMeta, AIRSTASHM_KEY_AUDIO_STREAM, nil);

                int64_t video_stream = airstashmeta_get_int64_l(rawMeta, AIRSTASHM_KEY_VIDEO_STREAM, -1);
                int64_t audio_stream = airstashmeta_get_int64_l(rawMeta, AIRSTASHM_KEY_AUDIO_STREAM, -1);

                NSMutableArray *streams = [[NSMutableArray alloc] init];

                size_t count = airstashmeta_get_children_count_l(rawMeta);
                for(size_t i = 0; i < count; ++i) {
                    AirStashMediaMeta *streamRawMeta = airstashmeta_get_child_l(rawMeta, i);
                    NSMutableDictionary *streamMeta = [[NSMutableDictionary alloc] init];

                    if (streamRawMeta) {
                        fillMetaInternal(streamMeta, streamRawMeta, AIRSTASHM_KEY_TYPE, k_AIRSTASHM_VAL_TYPE__UNKNOWN);
                        const char *type = airstashmeta_get_string_l(streamRawMeta, AIRSTASHM_KEY_TYPE);
                        if (type) {
                            fillMetaInternal(streamMeta, streamRawMeta, AIRSTASHM_KEY_CODEC_NAME, nil);
                            fillMetaInternal(streamMeta, streamRawMeta, AIRSTASHM_KEY_CODEC_PROFILE, nil);
                            fillMetaInternal(streamMeta, streamRawMeta, AIRSTASHM_KEY_CODEC_LONG_NAME, nil);
                            fillMetaInternal(streamMeta, streamRawMeta, AIRSTASHM_KEY_BITRATE, nil);

                            if (0 == strcmp(type, AIRSTASHM_VAL_TYPE__VIDEO)) {
                                fillMetaInternal(streamMeta, streamRawMeta, AIRSTASHM_KEY_WIDTH, nil);
                                fillMetaInternal(streamMeta, streamRawMeta, AIRSTASHM_KEY_HEIGHT, nil);
                                fillMetaInternal(streamMeta, streamRawMeta, AIRSTASHM_KEY_FPS_NUM, nil);
                                fillMetaInternal(streamMeta, streamRawMeta, AIRSTASHM_KEY_FPS_DEN, nil);
                                fillMetaInternal(streamMeta, streamRawMeta, AIRSTASHM_KEY_TBR_NUM, nil);
                                fillMetaInternal(streamMeta, streamRawMeta, AIRSTASHM_KEY_TBR_DEN, nil);
                                fillMetaInternal(streamMeta, streamRawMeta, AIRSTASHM_KEY_SAR_NUM, nil);
                                fillMetaInternal(streamMeta, streamRawMeta, AIRSTASHM_KEY_SAR_DEN, nil);

                                if (video_stream == i) {
                                    _videoMeta = streamMeta;

                                    int64_t fps_num = airstashmeta_get_int64_l(streamRawMeta, AIRSTASHM_KEY_FPS_NUM, 0);
                                    int64_t fps_den = airstashmeta_get_int64_l(streamRawMeta, AIRSTASHM_KEY_FPS_DEN, 0);
                                    if (fps_num > 0 && fps_den > 0) {
                                        _fpsInMeta = ((CGFloat)(fps_num)) / fps_den;
                                        NSLog(@"fps in meta %f\n", _fpsInMeta);
                                    }
                                }

                            } else if (0 == strcmp(type, AIRSTASHM_VAL_TYPE__AUDIO)) {
                                fillMetaInternal(streamMeta, streamRawMeta, AIRSTASHM_KEY_SAMPLE_RATE, nil);
                                fillMetaInternal(streamMeta, streamRawMeta, AIRSTASHM_KEY_CHANNEL_LAYOUT, nil);

                                if (audio_stream == i) {
                                    _audioMeta = streamMeta;
                                }

                            }
                        }
                    }

                    [streams addObject:streamMeta];
                }

                [newMediaMeta setObject:streams forKey:kk_AIRSTASHM_KEY_STREAMS];

                airstashmeta_unlock(rawMeta);
                _mediaMeta = newMediaMeta;
            }

            [self startHudTimer];
            _isPreparedToPlay = YES;

            [[NSNotificationCenter defaultCenter] postNotificationName:AirStashMPMediaPlaybackIsPreparedToPlayDidChangeNotification object:self];

            _loadState = AirStashMPMovieLoadStatePlayable | AirStashMPMovieLoadStatePlaythroughOK;

            [[NSNotificationCenter defaultCenter]
             postNotificationName:AirStashMPMoviePlayerLoadStateDidChangeNotification
             object:self];

            break;
        }
        case FFP_MSG_COMPLETED: {

            [self setScreenOn:NO];

            [[NSNotificationCenter defaultCenter]
             postNotificationName:AirStashMPMoviePlayerPlaybackStateDidChangeNotification
             object:self];

            [[NSNotificationCenter defaultCenter]
             postNotificationName:AirStashMPMoviePlayerPlaybackDidFinishNotification
             object:self
             userInfo:@{AirStashMPMoviePlayerPlaybackDidFinishReasonUserInfoKey: @(AirStashMPMovieFinishReasonPlaybackEnded)}];
            break;
        }
        case FFP_MSG_VIDEO_SIZE_CHANGED:
            NSLog(@"FFP_MSG_VIDEO_SIZE_CHANGED: %d, %d\n", avmsg->arg1, avmsg->arg2);
            if (avmsg->arg1 > 0)
                _videoWidth = avmsg->arg1;
            if (avmsg->arg2 > 0)
                _videoHeight = avmsg->arg2;
            [self changeNaturalSize];
            break;
        case FFP_MSG_SAR_CHANGED:
            NSLog(@"FFP_MSG_SAR_CHANGED: %d, %d\n", avmsg->arg1, avmsg->arg2);
            if (avmsg->arg1 > 0)
                _sampleAspectRatioNumerator = avmsg->arg1;
            if (avmsg->arg2 > 0)
                _sampleAspectRatioDenominator = avmsg->arg2;
            [self changeNaturalSize];
            break;
        case FFP_MSG_BUFFERING_START: {
            NSLog(@"FFP_MSG_BUFFERING_START:\n");

            _loadState = AirStashMPMovieLoadStateStalled;

            [[NSNotificationCenter defaultCenter]
             postNotificationName:AirStashMPMoviePlayerLoadStateDidChangeNotification
             object:self];
            break;
        }
        case FFP_MSG_BUFFERING_END: {
            NSLog(@"FFP_MSG_BUFFERING_END:\n");

            _loadState = AirStashMPMovieLoadStatePlayable | AirStashMPMovieLoadStatePlaythroughOK;

            [[NSNotificationCenter defaultCenter]
             postNotificationName:AirStashMPMoviePlayerLoadStateDidChangeNotification
             object:self];
            [[NSNotificationCenter defaultCenter]
             postNotificationName:AirStashMPMoviePlayerPlaybackStateDidChangeNotification
             object:self];
            break;
        }
        case FFP_MSG_BUFFERING_UPDATE:
            _bufferingPosition = avmsg->arg1;
            _bufferingProgress = avmsg->arg2;
            // NSLog(@"FFP_MSG_BUFFERING_UPDATE: %d, %%%d\n", _bufferingPosition, _bufferingProgress);
            break;
        case FFP_MSG_BUFFERING_BYTES_UPDATE:
            // NSLog(@"FFP_MSG_BUFFERING_BYTES_UPDATE: %d\n", avmsg->arg1);
            break;
        case FFP_MSG_BUFFERING_TIME_UPDATE:
            _bufferingTime       = avmsg->arg1;
            // NSLog(@"FFP_MSG_BUFFERING_TIME_UPDATE: %d\n", avmsg->arg1);
            break;
        case FFP_MSG_PLAYBACK_STATE_CHANGED:
            [[NSNotificationCenter defaultCenter]
             postNotificationName:AirStashMPMoviePlayerPlaybackStateDidChangeNotification
             object:self];
            break;
        case FFP_MSG_SEEK_COMPLETE: {
            NSLog(@"FFP_MSG_SEEK_COMPLETE:\n");
            _seeking = NO;
            break;
        }
        case FFP_MSG_VIDEO_DECODER_OPEN: {
            _isVideoToolboxOpen = avmsg->arg1;
            NSLog(@"FFP_MSG_VIDEO_DECODER_OPEN: %@\n", _isVideoToolboxOpen ? @"true" : @"false");
            [[NSNotificationCenter defaultCenter]
             postNotificationName:AirStashMPMoviePlayerVideoDecoderOpenNotification
             object:self];
            break;
        }
        case FFP_MSG_VIDEO_RENDERING_START: {
            NSLog(@"FFP_MSG_VIDEO_RENDERING_START:\n");
            [[NSNotificationCenter defaultCenter]
             postNotificationName:AirStashMPMoviePlayerFirstVideoFrameRenderedNotification
             object:self];
            break;
        }
        case FFP_MSG_AUDIO_RENDERING_START: {
            NSLog(@"FFP_MSG_AUDIO_RENDERING_START:\n");
            [[NSNotificationCenter defaultCenter]
             postNotificationName:AirStashMPMoviePlayerFirstAudioFrameRenderedNotification
             object:self];
            break;
        }
        default:
            // NSLog(@"unknown FFP_MSG_xxx(%d)\n", avmsg->what);
            break;
    }

    [_msgPool recycle:msg];
}

- (AirStashFFMoviePlayerMessage *) obtainMessage {
    return [_msgPool obtain];
}

inline static AirStashFFMoviePlayerController *ffplayerRetain(void *arg) {
    return (__bridge_transfer AirStashFFMoviePlayerController *) arg;
}

int media_player_msg_loop(void* arg)
{
    @autoreleasepool {
        AirStashMediaPlayer *mp = (AirStashMediaPlayer*)arg;
        __weak AirStashFFMoviePlayerController *ffpController = ffplayerRetain(airstashmp_set_weak_thiz(mp, NULL));

        while (ffpController) {
            @autoreleasepool {
                AirStashFFMoviePlayerMessage *msg = [ffpController obtainMessage];
                if (!msg)
                    break;

                int retval = airstashmp_get_msg(mp, &msg->_msg, 1);
                if (retval < 0)
                    break;

                // block-get should never return 0
                assert(retval > 0);

                [ffpController performSelectorOnMainThread:@selector(postEvent:) withObject:msg waitUntilDone:NO];
            }
        }

        // retained in prepare_async, before SDL_CreateThreadEx
        airstashmp_dec_ref_p(&mp);
        return 0;
    }
}

#pragma mark av_format_control_message

static int onInjectUrlOpen(AirStashFFMoviePlayerController *mpc, id<AirStashMediaUrlOpenDelegate> delegate, int type, void *data, size_t data_size)
{
    AirStashAVInject_OnUrlOpenData *realData = data;
    assert(realData);
    assert(sizeof(AirStashAVInject_OnUrlOpenData) == data_size);
    realData->is_handled     = NO;
    realData->is_url_changed = NO;

    if (delegate == nil)
        return 0;

    NSString *urlString = [NSString stringWithUTF8String:realData->url];
    NSURL *url = [NSURL URLWithString:urlString];
    if ([url.scheme isEqualToString:@"tcp"] || [url.scheme isEqualToString:@"udp"]) {
        if ([url.host airstash_isIpv4]) {
            [mpc->_glView setHudValue:url.host forKey:@"ip"];
        }
    } else {
        [mpc setHudUrl:urlString];
    }

    AirStashMediaUrlOpenData *openData =
    [[AirStashMediaUrlOpenData alloc] initWithUrl:urlString
                                    openType:(AirStashMediaUrlOpenType)type
                                segmentIndex:realData->segment_index
                                retryCounter:realData->retry_counter];

    [delegate willOpenUrl:openData];
    if (openData.error < 0)
        return -1;

    if (openData.isHandled) {
        realData->is_handled = YES;
        if (openData.isUrlChanged && openData.url != nil) {
            realData->is_url_changed = YES;
            const char *newUrlUTF8 = [openData.url UTF8String];
            strlcpy(realData->url, newUrlUTF8, sizeof(realData->url));
            realData->url[sizeof(realData->url) - 1] = 0;
        }
    }
    
    return 0;
}

static int onInjectAsyncStatistic(AirStashFFMoviePlayerController *mpc, int type, void *data, size_t data_size)
{
    AirStashAVInject_AsyncStatistic *realData = data;
    assert(realData);
    assert(sizeof(AirStashAVInject_AsyncStatistic) == data_size);

    mpc->_asyncStat = *realData;
    return 0;
}

// NOTE: could be called from multiple thread
static int airstashff_inject_callback(void *opaque, int message, void *data, size_t data_size)
{
    AirStashFFMoviePlayerController *mpc = (__bridge AirStashFFMoviePlayerController*)opaque;

    switch (message) {
        case AIRSTASHAVINJECT_CONCAT_RESOLVE_SEGMENT:
            return onInjectUrlOpen(mpc, mpc.segmentOpenDelegate, message, data, data_size);
        case AIRSTASHAVINJECT_ON_TCP_OPEN:
            return onInjectUrlOpen(mpc, mpc.tcpOpenDelegate, message, data, data_size);
        case AIRSTASHAVINJECT_ON_HTTP_OPEN:
            return onInjectUrlOpen(mpc, mpc.httpOpenDelegate, message, data, data_size);
        case AIRSTASHAVINJECT_ON_LIVE_RETRY:
            return onInjectUrlOpen(mpc, mpc.liveOpenDelegate, message, data, data_size);
        case AIRSTASHAVINJECT_ASYNC_STATISTIC:
            return onInjectAsyncStatistic(mpc, message, data, data_size);
        default: {
            return 0;
        }
    }
}

#pragma mark Airplay

-(BOOL)allowsMediaAirPlay
{
    if (!self)
        return NO;
    return _allowsMediaAirPlay;
}

-(void)setAllowsMediaAirPlay:(BOOL)b
{
    if (!self)
        return;
    _allowsMediaAirPlay = b;
}

-(BOOL)airPlayMediaActive
{
    if (!self)
        return NO;
    if (_isDanmakuMediaAirPlay) {
        return YES;
    }
    return NO;
}

-(BOOL)isDanmakuMediaAirPlay
{
    return _isDanmakuMediaAirPlay;
}

-(void)setIsDanmakuMediaAirPlay:(BOOL)isDanmakuMediaAirPlay
{
    _isDanmakuMediaAirPlay = isDanmakuMediaAirPlay;
    if (_isDanmakuMediaAirPlay) {
        _glView.scaleFactor = 1.0f;
    }
    else {
        CGFloat scale = [[UIScreen mainScreen] scale];
        if (scale < 0.1f)
            scale = 1.0f;
        _glView.scaleFactor = scale;
    }
     [[NSNotificationCenter defaultCenter] postNotificationName:AirStashMPMoviePlayerIsAirPlayVideoActiveDidChangeNotification object:nil userInfo:nil];
}


#pragma mark Option Conventionce

- (void)setFormatOptionValue:(NSString *)value forKey:(NSString *)key
{
    [self setOptionValue:value forKey:key ofCategory:kAirStashFFOptionCategoryFormat];
}

- (void)setCodecOptionValue:(NSString *)value forKey:(NSString *)key
{
    [self setOptionValue:value forKey:key ofCategory:kAirStashFFOptionCategoryCodec];
}

- (void)setSwsOptionValue:(NSString *)value forKey:(NSString *)key
{
    [self setOptionValue:value forKey:key ofCategory:kAirStashFFOptionCategorySws];
}

- (void)setPlayerOptionValue:(NSString *)value forKey:(NSString *)key
{
    [self setOptionValue:value forKey:key ofCategory:kAirStashFFOptionCategoryPlayer];
}

- (void)setFormatOptionIntValue:(int64_t)value forKey:(NSString *)key
{
    [self setOptionIntValue:value forKey:key ofCategory:kAirStashFFOptionCategoryFormat];
}

- (void)setCodecOptionIntValue:(int64_t)value forKey:(NSString *)key
{
    [self setOptionIntValue:value forKey:key ofCategory:kAirStashFFOptionCategoryCodec];
}

- (void)setSwsOptionIntValue:(int64_t)value forKey:(NSString *)key
{
    [self setOptionIntValue:value forKey:key ofCategory:kAirStashFFOptionCategorySws];
}

- (void)setPlayerOptionIntValue:(int64_t)value forKey:(NSString *)key
{
    [self setOptionIntValue:value forKey:key ofCategory:kAirStashFFOptionCategoryPlayer];
}

- (void)setMaxBufferSize:(int)maxBufferSize
{
    [self setPlayerOptionIntValue:maxBufferSize forKey:@"max-buffer-size"];
}


#pragma mark app state changed

- (void)registerApplicationObservers
{
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(audioSessionInterrupt:)
                                                 name:AVAudioSessionInterruptionNotification
                                               object:nil];
    [_registeredNotifications addObject:AVAudioSessionInterruptionNotification];

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(applicationWillEnterForeground)
                                                 name:UIApplicationWillEnterForegroundNotification
                                               object:nil];
    [_registeredNotifications addObject:UIApplicationWillEnterForegroundNotification];

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(applicationDidBecomeActive)
                                                 name:UIApplicationDidBecomeActiveNotification
                                               object:nil];
    [_registeredNotifications addObject:UIApplicationDidBecomeActiveNotification];

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(applicationWillResignActive)
                                                 name:UIApplicationWillResignActiveNotification
                                               object:nil];
    [_registeredNotifications addObject:UIApplicationWillResignActiveNotification];

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(applicationDidEnterBackground)
                                                 name:UIApplicationDidEnterBackgroundNotification
                                               object:nil];
    [_registeredNotifications addObject:UIApplicationDidEnterBackgroundNotification];

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(applicationWillTerminate)
                                                 name:UIApplicationWillTerminateNotification
                                               object:nil];
    [_registeredNotifications addObject:UIApplicationWillTerminateNotification];
}

- (void)unregisterApplicationObservers
{
    for (NSString *name in _registeredNotifications) {
        [[NSNotificationCenter defaultCenter] removeObserver:self
                                                        name:name
                                                      object:nil];
    }
}

- (void)audioSessionInterrupt:(NSNotification *)notification
{
    int reason = [[[notification userInfo] valueForKey:AVAudioSessionInterruptionTypeKey] intValue];
    switch (reason) {
        case AVAudioSessionInterruptionTypeBegan: {
            NSLog(@"AirStashFFMoviePlayerController:audioSessionInterrupt: begin\n");
            switch (self.playbackState) {
                case AirStashMPMoviePlaybackStatePaused:
                case AirStashMPMoviePlaybackStateStopped:
                    _playingBeforeInterruption = NO;
                    break;
                default:
                    _playingBeforeInterruption = YES;
                    break;
            }
            [self pause];
            [[AirStashAudioKit sharedInstance] setActive:NO];
            break;
        }
        case AVAudioSessionInterruptionTypeEnded: {
            NSLog(@"AirStashFFMoviePlayerController:audioSessionInterrupt: end\n");
            [[AirStashAudioKit sharedInstance] setActive:YES];
            if (_playingBeforeInterruption) {
                [self play];
            }
            break;
        }
    }
}

- (void)applicationWillEnterForeground
{
    NSLog(@"AirStashFFMoviePlayerController:applicationWillEnterForeground: %d", (int)[UIApplication sharedApplication].applicationState);
}

- (void)applicationDidBecomeActive
{
    NSLog(@"AirStashFFMoviePlayerController:applicationDidBecomeActive: %d", (int)[UIApplication sharedApplication].applicationState);
}

- (void)applicationWillResignActive
{
    NSLog(@"AirStashFFMoviePlayerController:applicationWillResignActive: %d", (int)[UIApplication sharedApplication].applicationState);
    dispatch_async(dispatch_get_main_queue(), ^{
        if (_pauseInBackground) {
            [self pause];
        }
    });
}

- (void)applicationDidEnterBackground
{
    NSLog(@"AirStashFFMoviePlayerController:applicationDidEnterBackground: %d", (int)[UIApplication sharedApplication].applicationState);
    dispatch_async(dispatch_get_main_queue(), ^{
        if (_pauseInBackground) {
            [self pause];
        }
    });
}

- (void)applicationWillTerminate
{
    NSLog(@"AirStashFFMoviePlayerController:applicationWillTerminate: %d", (int)[UIApplication sharedApplication].applicationState);
    dispatch_async(dispatch_get_main_queue(), ^{
        if (_pauseInBackground) {
            [self pause];
        }
    });
}

@end

