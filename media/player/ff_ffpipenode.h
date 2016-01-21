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

#ifndef FFPLAY__FF_FFPIPENODE_H
#define FFPLAY__FF_FFPIPENODE_H

#include "sdl/sdl_mutex.h"

typedef struct AirStashFF_Pipenode_Opaque AirStashFF_Pipenode_Opaque;
typedef struct AirStashFF_Pipenode AirStashFF_Pipenode;
struct AirStashFF_Pipenode {
    SDL_mutex *mutex;
    void *opaque;

    void (*func_destroy) (AirStashFF_Pipenode *node);
    int  (*func_run_sync)(AirStashFF_Pipenode *node);
    int  (*func_flush)   (AirStashFF_Pipenode *node); // optional
};

AirStashFF_Pipenode *ffpipenode_alloc(size_t opaque_size);
void ffpipenode_free(AirStashFF_Pipenode *node);
void ffpipenode_free_p(AirStashFF_Pipenode **node);

int  ffpipenode_run_sync(AirStashFF_Pipenode *node);
int  ffpipenode_flush(AirStashFF_Pipenode *node);

#endif
