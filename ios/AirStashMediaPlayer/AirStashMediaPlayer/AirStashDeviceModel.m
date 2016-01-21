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


#import "AirStashDeviceModel.h"

#include <sys/utsname.h>

@implementation AirStashDeviceModel

- (instancetype)initWithPlatform:(NSString *)platform
                        withName:(NSString *)name
                        withRank:(NSInteger)rank
{
    self = [super init];
    if (self) {
        _platform = platform;
        _name     = name;
        _rank     = rank;
    }
    return self;
}

inline static void AirStashDeviceRegister(NSMutableDictionary *dict,
                                    NSString *platform,
                                    NSString *name,
                                    NSInteger rank)
{
    AirStashDeviceModel *model = [[AirStashDeviceModel alloc] initWithPlatform:platform
                                                            withName:name
                                                            withRank:rank];
    [dict setObject:model forKey:platform];
}

+ (instancetype)modelWithName:(NSString *)name
{
    static AirStashDeviceModel  *sLatestModel = nil;
    static NSDictionary    *sModelDictionary = nil;
    static dispatch_once_t  sOnceToken = 0;
    dispatch_once(&sOnceToken, ^{
        NSMutableDictionary *dict = [[NSMutableDictionary alloc] init];

        // http://en.wikipedia.org/wiki/List_of_iOS_devices
        // http://www.everyiphone.com
        // Simulator
        AirStashDeviceRegister(dict, @"i386",        @"Simulator",       kAirStashDeviceRank_Simulator);
        AirStashDeviceRegister(dict, @"x86_64",      @"Simulator 64",    kAirStashDeviceRank_Simulator);

        // iPhone
        AirStashDeviceRegister(dict, @"iPhone1,1",   @"iPhone",          kAirStashDeviceRank_Baseline);
        AirStashDeviceRegister(dict, @"iPhone1,2",   @"iPhone 3G",       kAirStashDeviceRank_Baseline);

        AirStashDeviceRegister(dict, @"iPhone2,1",   @"iPhone 3GS",      kAirStashDeviceRank_Baseline);

        AirStashDeviceRegister(dict, @"iPhone3,1",   @"iPhone 4",        kAirStashDeviceRank_AppleA4Class);
        AirStashDeviceRegister(dict, @"iPhone3,2",   @"iPhone 4",        kAirStashDeviceRank_AppleA4Class);
        AirStashDeviceRegister(dict, @"iPhone3,3",   @"iPhone 4",        kAirStashDeviceRank_AppleA4Class);

        AirStashDeviceRegister(dict, @"iPhone4,1",   @"iPhone 4s",       kAirStashDeviceRank_AppleA5Class);

        AirStashDeviceRegister(dict, @"iPhone5,1",   @"iPhone 5",        kAirStashDeviceRank_AppleA6Class);
        AirStashDeviceRegister(dict, @"iPhone5,2",   @"iPhone 5",        kAirStashDeviceRank_AppleA6Class);
        AirStashDeviceRegister(dict, @"iPhone5,3",   @"iPhone 5c",       kAirStashDeviceRank_AppleA6Class);
        AirStashDeviceRegister(dict, @"iPhone5,4",   @"iPhone 5c",       kAirStashDeviceRank_AppleA6Class);

        AirStashDeviceRegister(dict, @"iPhone6,1",   @"iPhone 5s",       kAirStashDeviceRank_AppleA7Class);
        AirStashDeviceRegister(dict, @"iPhone6,2",   @"iPhone 5s",       kAirStashDeviceRank_AppleA7Class);

        AirStashDeviceRegister(dict, @"iPhone7,1",   @"iPhone 6 Plus",   kAirStashDeviceRank_AppleA8Class);
        AirStashDeviceRegister(dict, @"iPhone7,2",   @"iPhone 6",        kAirStashDeviceRank_AppleA8Class);

        AirStashDeviceRegister(dict, @"iPhone8,1",   @"iPhone 6S",       kAirStashDeviceRank_AppleA9Class);
        AirStashDeviceRegister(dict, @"iPhone8,2",   @"iPhone 6S Plus",  kAirStashDeviceRank_AppleA9Class);

        // iPod Touch
        AirStashDeviceRegister(dict, @"iPod1,1",     @"iPod Touch",      kAirStashDeviceRank_Baseline);
        AirStashDeviceRegister(dict, @"iPod2,1",     @"iPod Touch 2G",   kAirStashDeviceRank_Baseline);
        AirStashDeviceRegister(dict, @"iPod3,1",     @"iPod Touch 3G",   kAirStashDeviceRank_Baseline);

        AirStashDeviceRegister(dict, @"iPod4,1",     @"iPod Touch 4G",   kAirStashDeviceRank_AppleA4Class);

        AirStashDeviceRegister(dict, @"iPod5,1",     @"iPod Touch 5G",   kAirStashDeviceRank_AppleA5Class);

        AirStashDeviceRegister(dict, @"iPod7,1",     @"iPod Touch 6G",   kAirStashDeviceRank_AppleA8LowClass);

        // iPad / iPad mini
        AirStashDeviceRegister(dict, @"iPad1,1",     @"iPad",            kAirStashDeviceRank_AppleA4Class);

        AirStashDeviceRegister(dict, @"iPad2,1",     @"iPad 2",          kAirStashDeviceRank_AppleA5Class);
        AirStashDeviceRegister(dict, @"iPad2,2",     @"iPad 2",          kAirStashDeviceRank_AppleA5Class);
        AirStashDeviceRegister(dict, @"iPad2,3",     @"iPad 2",          kAirStashDeviceRank_AppleA5Class);

        AirStashDeviceRegister(dict, @"iPad2,4",     @"iPad 2",          kAirStashDeviceRank_AppleA5RAClass);
        AirStashDeviceRegister(dict, @"iPad2,5",     @"iPad mini",       kAirStashDeviceRank_AppleA5RAClass);
        AirStashDeviceRegister(dict, @"iPad2,6",     @"iPad mini",       kAirStashDeviceRank_AppleA5RAClass);
        AirStashDeviceRegister(dict, @"iPad2,7",     @"iPad mini",       kAirStashDeviceRank_AppleA5RAClass);

        AirStashDeviceRegister(dict, @"iPad3,1",     @"iPad 3G",         kAirStashDeviceRank_AppleA5XClass);
        AirStashDeviceRegister(dict, @"iPad3,2",     @"iPad 3G",         kAirStashDeviceRank_AppleA5XClass);
        AirStashDeviceRegister(dict, @"iPad3,3",     @"iPad 3G",         kAirStashDeviceRank_AppleA5XClass);

        AirStashDeviceRegister(dict, @"iPad3,4",     @"iPad 4G",         kAirStashDeviceRank_AppleA6XClass);
        AirStashDeviceRegister(dict, @"iPad3,5",     @"iPad 4G",         kAirStashDeviceRank_AppleA6XClass);
        AirStashDeviceRegister(dict, @"iPad3,6",     @"iPad 4G",         kAirStashDeviceRank_AppleA6XClass);

        AirStashDeviceRegister(dict, @"iPad4,1",     @"iPad Air",        kAirStashDeviceRank_AppleA7Class);
        AirStashDeviceRegister(dict, @"iPad4,2",     @"iPad Air",        kAirStashDeviceRank_AppleA7Class);
        AirStashDeviceRegister(dict, @"iPad4,3",     @"iPad Air",        kAirStashDeviceRank_AppleA7Class);
        AirStashDeviceRegister(dict, @"iPad4,4",     @"iPad mini 2G",    kAirStashDeviceRank_AppleA7Class);
        AirStashDeviceRegister(dict, @"iPad4,5",     @"iPad mini 2G",    kAirStashDeviceRank_AppleA7Class);
        AirStashDeviceRegister(dict, @"iPad4,6",     @"iPad mini 2G",    kAirStashDeviceRank_AppleA7Class);

        AirStashDeviceRegister(dict, @"iPad4,7",     @"iPad mini 3",     kAirStashDeviceRank_AppleA7Class);
        AirStashDeviceRegister(dict, @"iPad4,8",     @"iPad mini 3",     kAirStashDeviceRank_AppleA7Class);
        AirStashDeviceRegister(dict, @"iPad4,9",     @"iPad mini 3",     kAirStashDeviceRank_AppleA7Class);

        AirStashDeviceRegister(dict, @"iPad5,1",     @"iPad mini 4",     kAirStashDeviceRank_AppleA8XClass);
        AirStashDeviceRegister(dict, @"iPad5,2",     @"iPad mini 4",     kAirStashDeviceRank_AppleA8XClass);
        AirStashDeviceRegister(dict, @"iPad5,3",     @"iPad Air 2",      kAirStashDeviceRank_AppleA8XClass);
        AirStashDeviceRegister(dict, @"iPad5,4",     @"iPad Air 2",      kAirStashDeviceRank_AppleA8XClass);

        AirStashDeviceRegister(dict, @"iPad6,7",     @"iPad Pro",        kAirStashDeviceRank_AppleA9XClass);
        AirStashDeviceRegister(dict, @"iPad6,8",     @"iPad Pro",        kAirStashDeviceRank_AppleA9XClass);

        sModelDictionary = dict;

        sLatestModel = [[AirStashDeviceModel alloc] initWithPlatform:name
                                                       withName:name
                                                       withRank:kAirStashDeviceRank_LatestUnknown];
    });

    AirStashDeviceModel *model = [sModelDictionary objectForKey:name];
    if (model == nil) {
        if (model == nil) {
            model = sLatestModel;
        }

        NSArray *components = [name componentsSeparatedByString:@","];
        if (components != nil && components.count > 0) {
            NSString *majorName = components[0];
            if (majorName != nil) {
                majorName = [majorName stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
                model = [sModelDictionary objectForKey:majorName];
            }
        }
    }

    return model;
}

+ (NSString *)currentModelName
{
    struct utsname systemInfo;
    uname(&systemInfo);

    return [NSString stringWithUTF8String:systemInfo.machine];
}

+ (instancetype)currentModel
{
    return [AirStashDeviceModel modelWithName:[AirStashDeviceModel currentModelName]];
}

@end
