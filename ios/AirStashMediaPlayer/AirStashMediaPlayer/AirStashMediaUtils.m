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


#import "AirStashMediaUtils.h"

@implementation AirStashMediaUtils

+ (NSError*)createErrorWithDomain: (NSString*)domain
                             code: (NSInteger)code
                      description: (NSString*)description
                           reason: (NSString*)reason
{
    /* Generate an error describing the failure. */
    if (description == nil)
        description = @"";
    if (reason == nil)
        reason = @"";

    NSString *localizedDescription = NSLocalizedString(description, description);
    NSString *localizedFailureReason = NSLocalizedString(reason, reason);
    NSDictionary *errorDict = [NSDictionary dictionaryWithObjectsAndKeys:
                               localizedDescription, NSLocalizedDescriptionKey,
                               localizedFailureReason, NSLocalizedFailureReasonErrorKey,
                               nil];
    NSError *error = [NSError errorWithDomain:domain
                                         code:0
                                     userInfo:errorDict];
    return error;
}

@end
