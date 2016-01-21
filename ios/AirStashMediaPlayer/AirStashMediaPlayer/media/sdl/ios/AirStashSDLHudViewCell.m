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


#import "AirStashSDLHudViewCell.h"

#define COLUMN_COUNT    2
#define CELL_MARGIN     8

@interface AirStashSDLHudViewCell()

@end

@implementation AirStashSDLHudViewCell
{
    UILabel *_column[COLUMN_COUNT];
}

- (id)init
{
    self = [super init];
    if (self) {
        self.backgroundColor = [UIColor clearColor];
        self.contentView.backgroundColor = [UIColor clearColor];

        for (int i = 0; i < COLUMN_COUNT; ++i) {
            _column[i] = [[UILabel alloc] init];
            _column[i].textColor = [UIColor whiteColor];
            _column[i].font = [UIFont fontWithName:@"Menlo" size:9];

            [self.contentView addSubview:_column[i]];
        }
    }
    return self;
}

- (void)setHudValue:(NSString *)value forKey:(NSString *)key
{
    _column[0].text = key;
    _column[1].text = value;
}

- (void)layoutSubviews
{
    [super layoutSubviews];

    CGRect parentFrame = self.contentView.frame;
    CGRect newFrame    = parentFrame;
    CGFloat nextX      = CELL_MARGIN;

    newFrame.origin.x   = nextX;
    newFrame.size.width = parentFrame.size.width * 0.2;
    _column[0].frame    = newFrame;
    nextX               = newFrame.origin.x + newFrame.size.width + CELL_MARGIN;

    newFrame.origin.x   = nextX;
    newFrame.size.width = parentFrame.size.width - nextX - CELL_MARGIN;
    _column[1].frame = newFrame;
}

@end
