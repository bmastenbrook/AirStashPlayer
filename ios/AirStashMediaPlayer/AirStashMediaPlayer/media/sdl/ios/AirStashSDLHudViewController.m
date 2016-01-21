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


#import "AirStashSDLHudViewController.h"
#import "AirStashSDLHudViewCell.h"

@interface HudViewCellData : NSObject
@property(nonatomic) NSString *key;
@property(nonatomic) NSString *value;
@end

@implementation HudViewCellData
@end

@interface AirStashSDLHudViewController() <UITableViewDataSource, UITableViewDelegate>

@end

@implementation AirStashSDLHudViewController
{
    NSMutableDictionary *_keyIndexes;
    NSMutableArray      *_hudDataArray;
}

- (id)init
{
    self = [super init];
    if (self) {
        _keyIndexes = [[NSMutableDictionary alloc] init];
        _hudDataArray = [[NSMutableArray alloc] init];

        self.tableView.backgroundColor = [[UIColor alloc] initWithRed:.5f green:.5f blue:.5f alpha:.5f];
        self.tableView.separatorStyle  = UITableViewCellSeparatorStyleNone;
    }
    return self;
}

- (void)setHudValue:(NSString *)value forKey:(NSString *)key
{
    HudViewCellData *data = nil;
    NSNumber *index = [_keyIndexes objectForKey:key];
    if (index == nil) {
        data = [[HudViewCellData alloc] init];
        data.key = key;
        [_keyIndexes setObject:[NSNumber numberWithUnsignedInteger:_hudDataArray.count]
                        forKey:key];
        [_hudDataArray addObject:data];
    } else {
        data = [_hudDataArray objectAtIndex:[index unsignedIntegerValue]];
    }

    data.value = value;
    [self.tableView reloadData];
}

#pragma mark UITableViewDataSource

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    assert(section == 0);
    return _hudDataArray.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView
         cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    assert(indexPath.section == 0);

    AirStashSDLHudViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"hud"];
    if (cell == nil) {
        cell = [[AirStashSDLHudViewCell alloc] init];
    }

    HudViewCellData *data = [_hudDataArray objectAtIndex:indexPath.item];

    [cell setHudValue:data.value forKey:data.key];

    return cell;
}

#pragma mark UITableViewDelegate

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return 20.f;
}

@end
