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

typedef enum IAirStashFFOptionCategory {
    kAirStashFFOptionCategoryFormat = 1,
    kAirStashFFOptionCategoryCodec  = 2,
    kAirStashFFOptionCategorySws    = 3,
    kAirStashFFOptionCategoryPlayer = 4,
} AirStashFFOptionCategory;

// for codec option 'skip_loop_filter' and 'skip_frame'
typedef enum AirStashAVDiscard {
    /* We leave some space between them for extensions (drop some
     * keyframes for intra-only or drop just some bidir frames). */
    AIRSTASH_AVDISCARD_NONE    =-16, ///< discard nothing
    AIRSTASH_AVDISCARD_DEFAULT =  0, ///< discard useless packets like 0 size packets in avi
    AIRSTASH_AVDISCARD_NONREF  =  8, ///< discard all non reference
    AIRSTASH_AVDISCARD_BIDIR   = 16, ///< discard all bidirectional frames
    AIRSTASH_AVDISCARD_NONKEY  = 32, ///< discard all frames except keyframes
    AIRSTASH_AVDISCARD_ALL     = 48, ///< discard all
} AirStashAVDiscard;

struct AirStashMediaPlayer;

@interface AirStashFFOptions : NSObject

+(AirStashFFOptions *)optionsByDefault;

-(void)applyTo:(struct AirStashMediaPlayer *)mediaPlayer;

- (void)setOptionValue:(NSString *)value
                forKey:(NSString *)key
            ofCategory:(AirStashFFOptionCategory)category;

- (void)setOptionIntValue:(int64_t)value
                   forKey:(NSString *)key
               ofCategory:(AirStashFFOptionCategory)category;


-(void)setFormatOptionValue:       (NSString *)value forKey:(NSString *)key;
-(void)setCodecOptionValue:        (NSString *)value forKey:(NSString *)key;
-(void)setSwsOptionValue:          (NSString *)value forKey:(NSString *)key;
-(void)setPlayerOptionValue:       (NSString *)value forKey:(NSString *)key;

-(void)setFormatOptionIntValue:    (int64_t)value forKey:(NSString *)key;
-(void)setCodecOptionIntValue:     (int64_t)value forKey:(NSString *)key;
-(void)setSwsOptionIntValue:       (int64_t)value forKey:(NSString *)key;
-(void)setPlayerOptionIntValue:    (int64_t)value forKey:(NSString *)key;

@property(nonatomic) BOOL useRenderQueue;
@property(nonatomic) BOOL showHudView;

@end
