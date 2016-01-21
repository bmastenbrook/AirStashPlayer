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

#define kAirStashDeviceRank_Baseline                         10
#define kAirStashDeviceRank_AppleA4Class                     20   // Cortex-A8 class
#define kAirStashDeviceRank_AppleA5Class                     30   // Cortex-A9 class
#define kAirStashDeviceRank_AppleA5RAClass                   31   // Cortex-A9 class
#define kAirStashDeviceRank_AppleA5XClass                    35   // Cortex-A9 class
#define kAirStashDeviceRank_AppleA6Class                     40   // ARMv7s class
#define kAirStashDeviceRank_AppleA6XClass                    41   // ARMv7s class
#define kAirStashDeviceRank_AppleA7Class                     50   // ARM64 class
#define kAirStashDeviceRank_AppleA8LowClass                  55   // ARM64 class
#define kAirStashDeviceRank_AppleA8Class                     60   // ARM64 class
#define kAirStashDeviceRank_AppleA8XClass                    61   // ARM64 class
#define kAirStashDeviceRank_AppleA9Class                     70   // ARM64 class
#define kAirStashDeviceRank_AppleA9XClass                    71   // ARM64 class
#define kAirStashDeviceRank_LatestUnknown                    90
#define kAirStashDeviceRank_Simulator                        100

@interface AirStashDeviceModel : NSObject

@property(nonatomic) NSString   *platform;
@property(nonatomic) NSString   *name;
@property(nonatomic) NSInteger  rank;

+ (instancetype)currentModel;

@end
