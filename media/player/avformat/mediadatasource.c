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

#ifdef __ANDROID__

#include <assert.h>
#include "libavformat/avformat.h"
#include "libavformat/url.h"
#include "libavutil/avstring.h"
#include "libavutil/log.h"
#include "libavutil/opt.h"

#include "player/avutil/opt.h"
#include "avformat.h"

#include "sdl/android/sdl_android_jni.h"
#include "sdl/android/airstash/c/com/airstash/player/player/misc/IMediaDataSource.h"

typedef struct Context {
    AVClass        *class;

    /* options */
    int64_t         logical_pos;
    int64_t         logical_size;

    int64_t         media_data_source_ptr;
    jobject         media_data_source;
    jbyteArray      jbuffer;
    int             jbuffer_capacity;
} Context;

static int airstashmds_open(URLContext *h, const char *arg, int flags, AVDictionary **options)
{
    Context *c = h->priv_data;
    JNIEnv *env = NULL;
    jobject media_data_source = NULL;
    char *final = NULL;

    av_strstart(arg, "airstashmediadatasource:", &arg);

    media_data_source = (jobject) (intptr_t) strtoll(arg, &final, 10);
    if (!media_data_source)
        return AVERROR(EINVAL);

    if (JNI_OK != SDL_JNI_SetupThreadEnv(&env)) {
        av_log(h, AV_LOG_ERROR, "%s: SDL_JNI_SetupThreadEnv: failed", __func__);
        return AVERROR(EINVAL);
    }

    c->logical_size = AirStashC_IMediaDataSource__getSize(env, media_data_source);
    if (AirStash_ExceptionCheck__catchAll(env)) {
        return AVERROR(EINVAL);
    } else if (c->logical_size < 0) {
        h->is_streamed = 1;
        c->logical_size = -1;
    }

    c->media_data_source = (*env)->NewGlobalRef(env, media_data_source);
    if (AirStash_ExceptionCheck__catchAll(env) || !c->media_data_source) {
        return AVERROR(ENOMEM);
    }

    return 0;
}

static int airstashmds_close(URLContext *h)
{
    Context *c = h->priv_data;
    JNIEnv *env = NULL;

    if (JNI_OK != SDL_JNI_SetupThreadEnv(&env)) {
        av_log(h, AV_LOG_ERROR, "%s: SDL_JNI_SetupThreadEnv: failed", __func__);
        return AVERROR(EINVAL);
    }

    AirStash_DeleteGlobalRef__p(env, &c->jbuffer);

    if (c->media_data_source) {
        AirStashC_IMediaDataSource__close__catchAll(env, c->media_data_source);
        AirStash_DeleteGlobalRef__p(env, &c->media_data_source);
    }
    c->media_data_source_ptr = 0;

    return 0;
}

static jobject jbuffer_grow(JNIEnv *env, URLContext *h, int new_capacity) {
    Context *c = h->priv_data;

    if (c->jbuffer && c->jbuffer_capacity >= new_capacity)
        return c->jbuffer;

    new_capacity = FFMAX(new_capacity, c->jbuffer_capacity * 2);

    AirStash_DeleteGlobalRef__p(env, &c->jbuffer);
    c->jbuffer_capacity = 0;

    c->jbuffer = AirStash_NewByteArray__asGlobalRef__catchAll(env, new_capacity);
    if (AirStash_ExceptionCheck__catchAll(env) || !c->jbuffer) {
        c->jbuffer = NULL;
        return NULL;
    }

    c->jbuffer_capacity = new_capacity;
    return c->jbuffer;
}

static int airstashmds_read(URLContext *h, unsigned char *buf, int size)
{
    Context    *c = h->priv_data;
    JNIEnv     *env = NULL;
    jbyteArray  jbuffer = NULL;
    jint        ret = 0;

    if (!c->media_data_source) 
        return AVERROR(EINVAL);

    if (JNI_OK != SDL_JNI_SetupThreadEnv(&env)) {
        av_log(h, AV_LOG_ERROR, "%s: SDL_JNI_SetupThreadEnv: failed", __func__);
        return AVERROR(EINVAL);
    }

    jbuffer = jbuffer_grow(env, h, size);
    if (!jbuffer)
        return AVERROR(ENOMEM);

    ret = AirStashC_IMediaDataSource__readAt(env, c->media_data_source, c->logical_pos, jbuffer, 0, size);
    if (AirStash_ExceptionCheck__catchAll(env))
        return AVERROR(EIO);
    else if (ret < 0)
        return AVERROR_EOF;
    else if (ret == 0)
        return AVERROR(EAGAIN);

    (*env)->GetByteArrayRegion(env, jbuffer, 0, ret, (jbyte*)buf);
    if (AirStash_ExceptionCheck__catchAll(env))
        return AVERROR(EIO);

    c->logical_pos += ret;
    return ret;
}

static int64_t airstashmds_seek(URLContext *h, int64_t pos, int whence)
{
    Context *c = h->priv_data;
    int64_t  ret;
    int64_t  new_logical_pos;
    JNIEnv  *env = NULL;
    jobject  jbuffer = NULL;

    if (!c->media_data_source) 
        return AVERROR(EINVAL);

    if (JNI_OK != SDL_JNI_SetupThreadEnv(&env)) {
        av_log(h, AV_LOG_ERROR, "%s: SDL_JNI_SetupThreadEnv: failed", __func__);
        return AVERROR(EINVAL);
    }

    if (whence == AVSEEK_SIZE) {
        av_log(h, AV_LOG_TRACE, "%s: AVSEEK_SIZE: %"PRId64"\n", __func__, (int64_t)c->logical_size);
        return c->logical_size;
    } else if (whence == SEEK_CUR) {
        av_log(h, AV_LOG_TRACE, "%s: %"PRId64"\n", __func__, pos);
        new_logical_pos = pos + c->logical_pos;
    } else if (whence == SEEK_SET){
        av_log(h, AV_LOG_TRACE, "%s: %"PRId64"\n", __func__, pos);
        new_logical_pos = pos;
    } else {
        return AVERROR(EINVAL);
    }
    if (new_logical_pos < 0)
        return AVERROR(EINVAL);

    jbuffer = jbuffer_grow(env, h, 0);
    if (!jbuffer)
        return AVERROR(ENOMEM);

    ret = AirStashC_IMediaDataSource__readAt(env, c->media_data_source, new_logical_pos, jbuffer, 0, 0);
    if (AirStash_ExceptionCheck__catchAll(env))
        return AVERROR(EIO);
    else if (ret < 0)
        return AVERROR_EOF;

    c->logical_pos = new_logical_pos;
    return c->logical_pos;
}

#define OFFSET(x) offsetof(Context, x)
#define D AV_OPT_FLAG_DECODING_PARAM

static const AVOption options[] = {
    { NULL }
};

#undef D
#undef OFFSET

static const AVClass airstashmediadatasource_context_class = {
    .class_name = "airstashMediaDataSource",
    .item_name  = av_default_item_name,
    .option     = options,
    .version    = LIBAVUTIL_VERSION_INT,
};

URLProtocol airstashff_airstashmediadatasource_protocol = {
    .name                = "airstashmediadatasource",
    .url_open2           = airstashmds_open,
    .url_read            = airstashmds_read,
    .url_seek            = airstashmds_seek,
    .url_close           = airstashmds_close,
    .priv_data_size      = sizeof(Context),
    .priv_data_class     = &airstashmediadatasource_context_class,
};

#endif
