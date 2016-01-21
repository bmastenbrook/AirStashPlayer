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


#import "AirStashSDLAudioUnitController.h"
#import "AirStashSDLAudioKit.h"
#include "sdl/sdl_log.h"

#import <AVFoundation/AVFoundation.h>

@implementation AirStashSDLAudioUnitController {
    AudioUnit _auUnit;
    BOOL _isPaused;
}

- (id)initWithAudioSpec:(const SDL_AudioSpec *)aSpec
{
    self = [super init];
    if (self) {
        if (aSpec == NULL) {
            self = nil;
            return nil;
        }
        _spec = *aSpec;

        AudioComponentDescription desc;
        AirStashSDLGetAudioComponentDescriptionFromSpec(&_spec, &desc);

        AudioComponent auComponent = AudioComponentFindNext(NULL, &desc);
        if (auComponent == NULL) {
            ALOGE("AudioUnit: AudioComponentFindNext failed");
            self = nil;
            return nil;
        }

        AudioUnit auUnit;
        OSStatus status = AudioComponentInstanceNew(auComponent, &auUnit);
        if (status != noErr) {
            ALOGE("AudioUnit: AudioComponentInstanceNew failed");
            self = nil;
            return nil;
        }

        UInt32 flag = 1;
        status = AudioUnitSetProperty(auUnit,
                                      kAudioOutputUnitProperty_EnableIO,
                                      kAudioUnitScope_Output,
                                      0,
                                      &flag,
                                      sizeof(flag));
        if (status != noErr) {
            ALOGE("AudioUnit: failed to set IO mode (%d)", (int)status);
        }

        /* Get the current format */
        _spec.format = AUDIO_S16SYS;
        _spec.channels = 2;
        AudioStreamBasicDescription streamDescription;
        AirStashSDLGetAudioStreamBasicDescriptionFromSpec(&_spec, &streamDescription);

        /* Set the desired format */
        UInt32 i_param_size = sizeof(streamDescription);
        status = AudioUnitSetProperty(auUnit,
                                      kAudioUnitProperty_StreamFormat,
                                      kAudioUnitScope_Input,
                                      0,
                                      &streamDescription,
                                      i_param_size);
        if (status != noErr) {
            ALOGE("AudioUnit: failed to set stream format (%d)", (int)status);
            self = nil;
            return nil;
        }

        /* Retrieve actual format */
        status = AudioUnitGetProperty(auUnit,
                                      kAudioUnitProperty_StreamFormat,
                                      kAudioUnitScope_Input,
                                      0,
                                      &streamDescription,
                                      &i_param_size);
        if (status != noErr) {
            ALOGE("AudioUnit: failed to verify stream format (%d)\n", (int)status);
        }

        AURenderCallbackStruct callback;
        callback.inputProc = (AURenderCallback) RenderCallback;
        callback.inputProcRefCon = (__bridge void*) self;
        status = AudioUnitSetProperty(auUnit,
                                      kAudioUnitProperty_SetRenderCallback,
                                      kAudioUnitScope_Input,
                                      0, &callback, sizeof(callback));
        if (status != noErr) {
            ALOGE("AudioUnit: render callback setup failed (%d)\n", (int)status);
            self = nil;
            return nil;
        }

        SDL_CalculateAudioSpec(&_spec);

        /* AU initiliaze */
        status = AudioUnitInitialize(auUnit);
        if (status != noErr) {
            ALOGE("AudioUnit: AudioUnitInitialize failed (%d)\n", (int)status);
            self = nil;
            return nil;
        }

        _auUnit = auUnit;
    }
    return self;
}

- (void)dealloc
{
    [self close];
}

- (void)play
{
    if (!_auUnit)
        return;

    _isPaused = NO;
    NSError *error = nil;
    if (NO == [[AVAudioSession sharedInstance] setActive:YES error:&error]) {
        NSLog(@"AudioUnit: AVAudioSession.setActive(YES) failed: %@\n", error ? [error localizedDescription] : @"nil");
    }

    OSStatus status = AudioOutputUnitStart(_auUnit);
    if (status != noErr)
        NSLog(@"AudioUnit: AudioOutputUnitStart failed (%d)\n", (int)status);
}

- (void)pause
{
    if (!_auUnit)
        return;

    _isPaused = YES;
    OSStatus status = AudioOutputUnitStop(_auUnit);
    if (status != noErr)
        ALOGE("AudioUnit: failed to stop AudioUnit (%d)\n", (int)status);
}

- (void)flush
{
    if (!_auUnit)
        return;

    AudioUnitReset(_auUnit, kAudioUnitScope_Global, 0);
}

- (void)stop
{
    if (!_auUnit)
        return;

    OSStatus status = AudioOutputUnitStop(_auUnit);
    if (status != noErr)
        ALOGE("AudioUnit: failed to stop AudioUnit (%d)", (int)status);
}

- (void)close
{
    [self stop];

    if (!_auUnit)
        return;

    AURenderCallbackStruct callback;
    memset(&callback, 0, sizeof(AURenderCallbackStruct));
    AudioUnitSetProperty(_auUnit,
                         kAudioUnitProperty_SetRenderCallback,
                         kAudioUnitScope_Input, 0, &callback,
                         sizeof(callback));

    AudioComponentInstanceDispose(_auUnit);
    _auUnit = NULL;
}

static OSStatus RenderCallback(void                        *inRefCon,
                               AudioUnitRenderActionFlags  *ioActionFlags,
                               const AudioTimeStamp        *inTimeStamp,
                               UInt32                      inBusNumber,
                               UInt32                      inNumberFrames,
                               AudioBufferList             *ioData)
{
    @autoreleasepool {
        AirStashSDLAudioUnitController* auController = (__bridge AirStashSDLAudioUnitController *) inRefCon;

        if (!auController || auController->_isPaused) {
            for (UInt32 i = 0; i < ioData->mNumberBuffers; i++) {
                AudioBuffer *ioBuffer = &ioData->mBuffers[i];
                memset(ioBuffer->mData, auController.spec.silence, ioBuffer->mDataByteSize);
            }
            return noErr;
        }

        for (int i = 0; i < (int)ioData->mNumberBuffers; i++) {
            AudioBuffer *ioBuffer = &ioData->mBuffers[i];
            (*auController.spec.callback)(auController.spec.userdata, ioBuffer->mData, ioBuffer->mDataByteSize);
        }

        return noErr;
    }
}

@end
