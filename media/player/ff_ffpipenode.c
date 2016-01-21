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

#include "ff_ffpipenode.h"
#include <stdlib.h>
#include <string.h>

AirStashFF_Pipenode *ffpipenode_alloc(size_t opaque_size)
{
    AirStashFF_Pipenode *node = (AirStashFF_Pipenode*) calloc(1, sizeof(AirStashFF_Pipenode));
    if (!node)
        return NULL;

    node->opaque = calloc(1, opaque_size);
    if (!node->opaque) {
        free(node);
        return NULL;
    }

    node->mutex = SDL_CreateMutex();
    if (node->mutex == NULL) {
        free(node->opaque);
        free(node);
        return NULL;
    }

    return node;
}

void ffpipenode_free(AirStashFF_Pipenode *node)
{
    if (!node)
        return;

    if (node->func_destroy) {
        node->func_destroy(node);
    }

    SDL_DestroyMutexP(&node->mutex);

    free(node->opaque);
    memset(node, 0, sizeof(AirStashFF_Pipenode));
    free(node);
}

void ffpipenode_free_p(AirStashFF_Pipenode **node)
{
    if (!node)
        return;

    ffpipenode_free(*node);
    *node = NULL;
}

int ffpipenode_run_sync(AirStashFF_Pipenode *node)
{
    return node->func_run_sync(node);
}

int ffpipenode_flush(AirStashFF_Pipenode *node)
{
    if (!node || !node->func_flush)
        return 0;

    return node->func_flush(node);
}
