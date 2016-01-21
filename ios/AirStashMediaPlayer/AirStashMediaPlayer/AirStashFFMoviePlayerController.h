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
#import "AirStashFFOptions.h"

// media meta
#define k_AIRSTASHM_KEY_FORMAT         @"format"
#define k_AIRSTASHM_KEY_DURATION_US    @"duration_us"
#define k_AIRSTASHM_KEY_START_US       @"start_us"
#define k_AIRSTASHM_KEY_BITRATE        @"bitrate"

// stream meta
#define k_AIRSTASHM_KEY_TYPE           @"type"
#define k_AIRSTASHM_VAL_TYPE__VIDEO    @"video"
#define k_AIRSTASHM_VAL_TYPE__AUDIO    @"audio"
#define k_AIRSTASHM_VAL_TYPE__UNKNOWN  @"unknown"

#define k_AIRSTASHM_KEY_CODEC_NAME      @"codec_name"
#define k_AIRSTASHM_KEY_CODEC_PROFILE   @"codec_profile"
#define k_AIRSTASHM_KEY_CODEC_LONG_NAME @"codec_long_name"

// stream: video
#define k_AIRSTASHM_KEY_WIDTH          @"width"
#define k_AIRSTASHM_KEY_HEIGHT         @"height"
#define k_AIRSTASHM_KEY_FPS_NUM        @"fps_num"
#define k_AIRSTASHM_KEY_FPS_DEN        @"fps_den"
#define k_AIRSTASHM_KEY_TBR_NUM        @"tbr_num"
#define k_AIRSTASHM_KEY_TBR_DEN        @"tbr_den"
#define k_AIRSTASHM_KEY_SAR_NUM        @"sar_num"
#define k_AIRSTASHM_KEY_SAR_DEN        @"sar_den"
// stream: audio
#define k_AIRSTASHM_KEY_SAMPLE_RATE    @"sample_rate"
#define k_AIRSTASHM_KEY_CHANNEL_LAYOUT @"channel_layout"

#define kk_AIRSTASHM_KEY_STREAMS       @"streams"

typedef enum AirStashLogLevel {
    k_AIRSTASH_LOG_UNKNOWN = 0,
    k_AIRSTASH_LOG_DEFAULT = 1,

    k_AIRSTASH_LOG_VERBOSE = 2,
    k_AIRSTASH_LOG_DEBUG   = 3,
    k_AIRSTASH_LOG_INFO    = 4,
    k_AIRSTASH_LOG_WARN    = 5,
    k_AIRSTASH_LOG_ERROR   = 6,
    k_AIRSTASH_LOG_FATAL   = 7,
    k_AIRSTASH_LOG_SILENT  = 8,
} AirStashLogLevel;

@interface AirStashFFMoviePlayerController : NSObject <AirStashMediaPlayback>

- (id)initWithContentURL:(NSURL *)aUrl
             withOptions:(AirStashFFOptions *)options;

- (id)initWithContentURLString:(NSString *)aUrlString
                   withOptions:(AirStashFFOptions *)options;

- (void)prepareToPlay;
- (void)play;
- (void)pause;
- (void)stop;
- (BOOL)isPlaying;

- (void)setPauseInBackground:(BOOL)pause;
- (BOOL)isVideoToolboxOpen;

+ (void)setLogReport:(BOOL)preferLogReport;
+ (void)setLogLevel:(AirStashLogLevel)logLevel;
+ (BOOL)checkIfFFmpegVersionMatch:(BOOL)showAlert;
+ (BOOL)checkIfPlayerVersionMatch:(BOOL)showAlert
                            major:(unsigned int)major
                            minor:(unsigned int)minor
                            micro:(unsigned int)micro;

@property(nonatomic, readonly) CGFloat fpsInMeta;
@property(nonatomic, readonly) CGFloat fpsAtOutput;
@property(nonatomic) BOOL shouldShowHudView;

- (void)setOptionValue:(NSString *)value
                forKey:(NSString *)key
            ofCategory:(AirStashFFOptionCategory)category;

- (void)setOptionIntValue:(int64_t)value
                   forKey:(NSString *)key
               ofCategory:(AirStashFFOptionCategory)category;



- (void)setFormatOptionValue:       (NSString *)value forKey:(NSString *)key;
- (void)setCodecOptionValue:        (NSString *)value forKey:(NSString *)key;
- (void)setSwsOptionValue:          (NSString *)value forKey:(NSString *)key;
- (void)setPlayerOptionValue:       (NSString *)value forKey:(NSString *)key;

- (void)setFormatOptionIntValue:    (int64_t)value forKey:(NSString *)key;
- (void)setCodecOptionIntValue:     (int64_t)value forKey:(NSString *)key;
- (void)setSwsOptionIntValue:       (int64_t)value forKey:(NSString *)key;
- (void)setPlayerOptionIntValue:    (int64_t)value forKey:(NSString *)key;

@property (nonatomic, retain) id<AirStashMediaUrlOpenDelegate> segmentOpenDelegate;
@property (nonatomic, retain) id<AirStashMediaUrlOpenDelegate> tcpOpenDelegate;
@property (nonatomic, retain) id<AirStashMediaUrlOpenDelegate> httpOpenDelegate;
@property (nonatomic, retain) id<AirStashMediaUrlOpenDelegate> liveOpenDelegate;

@end

#define AIRSTASH_FF_IO_TYPE_READ (1)
void AirStashFFIOStatDebugCallback(const char *url, int type, int bytes);
void AirStashFFIOStatRegister(void (*cb)(const char *url, int type, int bytes));

void AirStashFFIOStatCompleteDebugCallback(const char *url,
                                      int64_t read_bytes, int64_t total_size,
                                      int64_t elpased_time, int64_t total_duration);
void AirStashFFIOStatCompleteRegister(void (*cb)(const char *url,
                                            int64_t read_bytes, int64_t total_size,
                                            int64_t elpased_time, int64_t total_duration));
