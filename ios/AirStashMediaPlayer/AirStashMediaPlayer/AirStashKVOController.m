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


#import "AirStashKVOController.h"

@interface AirStashKVOEntry : NSObject
@property(nonatomic, weak)   NSObject *observer;
@property(nonatomic, strong) NSString *keyPath;
@end

@implementation AirStashKVOEntry
@synthesize observer;
@synthesize keyPath;
@end

@implementation AirStashKVOController {
    __weak NSObject *_target;
    NSMutableArray  *_observerArray;
}

- (id)initWithTarget:(NSObject *)target
{
    self = [super init];
    if (self) {
        _target = target;
        _observerArray = [[NSMutableArray alloc] init];
    }
    return self;
}

- (void)safelyAddObserver:(NSObject *)observer
               forKeyPath:(NSString *)keyPath
                  options:(NSKeyValueObservingOptions)options
                  context:(void *)context
{
    NSObject *target = _target;
    if (target == nil)
        return;

    BOOL removed = [self removeEntryOfObserver:observer forKeyPath:keyPath];
    if (removed) {
        // duplicated register
        NSLog(@"duplicated observer");
    }

    @try {
        [target addObserver:observer
                 forKeyPath:keyPath
                    options:options
                    context:context];
        
        AirStashKVOEntry *entry = [[AirStashKVOEntry alloc] init];
        entry.observer = observer;
        entry.keyPath  = keyPath;
        [_observerArray addObject:entry];
    } @catch (NSException *e) {
        NSLog(@"AirStashKVO: failed to add observer for %@\n", keyPath);
    }
}

- (void)safelyRemoveObserver:(NSObject *)observer
                  forKeyPath:(NSString *)keyPath
{
    NSObject *target = _target;
    if (target == nil)
        return;

    BOOL removed = [self removeEntryOfObserver:observer forKeyPath:keyPath];
    if (removed) {
        // duplicated register
        NSLog(@"duplicated observer");
    }

    @try {
        if (removed) {
            [target removeObserver:observer
                        forKeyPath:keyPath];
        }
    } @catch (NSException *e) {
        NSLog(@"AirStashKVO: failed to remove observer for %@\n", keyPath);
    }
}

- (void)safelyRemoveAllObservers
{
    __block NSObject *target = _target;
    if (target == nil)
        return;

    [_observerArray enumerateObjectsUsingBlock:^(id obj, NSUInteger idx, BOOL *stop) {
        AirStashKVOEntry *entry = obj;
        if (entry == nil)
            return;

        NSObject *observer = entry.observer;
        if (observer == nil)
            return;

        @try {
            [target removeObserver:observer
                        forKeyPath:entry.keyPath];
        } @catch (NSException *e) {
            NSLog(@"AirStashKVO: failed to remove observer for %@\n", entry.keyPath);
        }
    }];

    [_observerArray removeAllObjects];
}

- (BOOL)removeEntryOfObserver:(NSObject *)observer
                   forKeyPath:(NSString *)keyPath
{
    __block NSInteger foundIndex = -1;
    [_observerArray enumerateObjectsUsingBlock:^(id obj, NSUInteger idx, BOOL *stop) {
        AirStashKVOEntry *entry = (AirStashKVOEntry *)obj;
        if (entry.observer == observer &&
            [entry.keyPath isEqualToString:keyPath]) {
            foundIndex = idx;
            *stop = YES;
        }
    }];

    if (foundIndex >= 0) {
        [_observerArray removeObjectAtIndex:foundIndex];
        return YES;
    }

    return NO;
}

@end
