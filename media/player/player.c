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

#include "player.h"
#include "player_internal.h"
#include "version.h"

#define MP_RET_IF_FAILED(ret) \
    do { \
        int retval = ret; \
        if (retval != 0) return (retval); \
    } while(0)

#define MPST_RET_IF_EQ_INT(real, expected, errcode) \
    do { \
        if ((real) == (expected)) return (errcode); \
    } while(0)

#define MPST_RET_IF_EQ(real, expected) \
    MPST_RET_IF_EQ_INT(real, expected, EAIRSTASH_INVALID_STATE)

inline static void airstashmp_destroy(AirStashMediaPlayer *mp)
{
    if (!mp)
        return;

    ffp_destroy_p(&mp->ffplayer);
    if (mp->msg_thread) {
        SDL_WaitThread(mp->msg_thread, NULL);
        mp->msg_thread = NULL;
    }

    pthread_mutex_destroy(&mp->mutex);

    freep((void**)&mp->data_source);
    memset(mp, 0, sizeof(AirStashMediaPlayer));
    freep((void**)&mp);
}

inline static void airstashmp_destroy_p(AirStashMediaPlayer **pmp)
{
    if (!pmp)
        return;

    airstashmp_destroy(*pmp);
    *pmp = NULL;
}

void airstashmp_global_init()
{
    ffp_global_init();
}

void airstashmp_global_uninit()
{
    ffp_global_uninit();
}

void airstashmp_global_set_log_report(int use_report)
{
    ffp_global_set_log_report(use_report);
}

void airstashmp_global_set_log_level(int log_level)
{
    ffp_global_set_log_level(log_level);
}

void airstashmp_global_set_inject_callback(airstash_inject_callback cb)
{
    ffp_global_set_inject_callback(cb);
}

const char *airstashmp_version_ident()
{
    return LIBAIRSTASHPLAYER_IDENT;
}

unsigned int airstashmp_version_int()
{
    return LIBAIRSTASHPLAYER_VERSION_INT;
}

void airstashmp_io_stat_register(void (*cb)(const char *url, int type, int bytes))
{
    ffp_io_stat_register(cb);
}

void airstashmp_io_stat_complete_register(void (*cb)(const char *url,
                                                int64_t read_bytes, int64_t total_size,
                                                int64_t elpased_time, int64_t total_duration))
{
    ffp_io_stat_complete_register(cb);
}

void airstashmp_change_state_l(AirStashMediaPlayer *mp, int new_state)
{
    mp->mp_state = new_state;
    ffp_notify_msg1(mp->ffplayer, FFP_MSG_PLAYBACK_STATE_CHANGED);
}

AirStashMediaPlayer *airstashmp_create(int (*msg_loop)(void*))
{
    AirStashMediaPlayer *mp = (AirStashMediaPlayer *) mallocz(sizeof(AirStashMediaPlayer));
    if (!mp)
        goto fail;

    mp->ffplayer = ffp_create();
    if (!mp->ffplayer)
        goto fail;

    mp->msg_loop = msg_loop;

    airstashmp_inc_ref(mp);
    pthread_mutex_init(&mp->mutex, NULL);

    return mp;

    fail:
    airstashmp_destroy_p(&mp);
    return NULL;
}

void airstashmp_set_inject_opaque(AirStashMediaPlayer *mp, void *opaque)
{
    assert(mp);

    MPTRACE("%s(%p)\n", __func__, opaque);
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#endif
    airstashmp_set_option_int(mp, AIRSTASHMP_OPT_CATEGORY_FORMAT, "airstashinject-opaque", (int64_t)opaque);
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
    MPTRACE("%s()=void\n", __func__);
}

void airstashmp_set_option(AirStashMediaPlayer *mp, int opt_category, const char *name, const char *value)
{
    assert(mp);

    // MPTRACE("%s(%s, %s)\n", __func__, name, value);
    pthread_mutex_lock(&mp->mutex);
    ffp_set_option(mp->ffplayer, opt_category, name, value);
    pthread_mutex_unlock(&mp->mutex);
    // MPTRACE("%s()=void\n", __func__);
}

void airstashmp_set_option_int(AirStashMediaPlayer *mp, int opt_category, const char *name, int64_t value)
{
    assert(mp);

    // MPTRACE("%s(%s, %"PRId64")\n", __func__, name, value);
    pthread_mutex_lock(&mp->mutex);
    ffp_set_option_int(mp->ffplayer, opt_category, name, value);
    pthread_mutex_unlock(&mp->mutex);
    // MPTRACE("%s()=void\n", __func__);
}

int airstashmp_get_video_codec_info(AirStashMediaPlayer *mp, char **codec_info)
{
    assert(mp);

    MPTRACE("%s\n", __func__);
    pthread_mutex_lock(&mp->mutex);
    int ret = ffp_get_video_codec_info(mp->ffplayer, codec_info);
    pthread_mutex_unlock(&mp->mutex);
    MPTRACE("%s()=void\n", __func__);
    return ret;
}

int airstashmp_get_audio_codec_info(AirStashMediaPlayer *mp, char **codec_info)
{
    assert(mp);

    MPTRACE("%s\n", __func__);
    pthread_mutex_lock(&mp->mutex);
    int ret = ffp_get_audio_codec_info(mp->ffplayer, codec_info);
    pthread_mutex_unlock(&mp->mutex);
    MPTRACE("%s()=void\n", __func__);
    return ret;
}

void airstashmp_set_playback_rate(AirStashMediaPlayer *mp, float rate)
{
    assert(mp);

    MPTRACE("%s(%f)\n", __func__, rate);
    pthread_mutex_lock(&mp->mutex);
    ffp_set_playback_rate(mp->ffplayer, rate);
    pthread_mutex_unlock(&mp->mutex);
    MPTRACE("%s()=void\n", __func__);
}

int airstashmp_set_stream_selected(AirStashMediaPlayer *mp, int stream, int selected)
{
    assert(mp);

    MPTRACE("%s(%d, %d)\n", __func__, stream, selected);
    pthread_mutex_lock(&mp->mutex);
    int ret = ffp_set_stream_selected(mp->ffplayer, stream, selected);
    pthread_mutex_unlock(&mp->mutex);
    MPTRACE("%s(%d, %d)=%d\n", __func__, stream, selected, ret);
    return ret;
}

float airstashmp_get_property_float(AirStashMediaPlayer *mp, int id, float default_value)
{
    assert(mp);

    pthread_mutex_lock(&mp->mutex);
    float ret = ffp_get_property_float(mp->ffplayer, id, default_value);
    pthread_mutex_unlock(&mp->mutex);
    return ret;
}

void airstashmp_set_property_float(AirStashMediaPlayer *mp, int id, float value)
{
    assert(mp);

    pthread_mutex_lock(&mp->mutex);
    ffp_set_property_float(mp->ffplayer, id, value);
    pthread_mutex_unlock(&mp->mutex);
}

int64_t airstashmp_get_property_int64(AirStashMediaPlayer *mp, int id, int64_t default_value)
{
    assert(mp);

    pthread_mutex_lock(&mp->mutex);
    int64_t ret = ffp_get_property_int64(mp->ffplayer, id, default_value);
    pthread_mutex_unlock(&mp->mutex);
    return ret;
}

void airstashmp_set_property_int64(AirStashMediaPlayer *mp, int id, int64_t value)
{
    assert(mp);

    pthread_mutex_lock(&mp->mutex);
    ffp_set_property_int64(mp->ffplayer, id, value);
    pthread_mutex_unlock(&mp->mutex);
}

AirStashMediaMeta *airstashmp_get_meta_l(AirStashMediaPlayer *mp)
{
    assert(mp);

    MPTRACE("%s\n", __func__);
    AirStashMediaMeta *ret = ffp_get_meta_l(mp->ffplayer);
    MPTRACE("%s()=void\n", __func__);
    return ret;
}

void airstashmp_shutdown_l(AirStashMediaPlayer *mp)
{
    assert(mp);

    MPTRACE("airstashmp_shutdown_l()\n");
    if (mp->ffplayer) {
        ffp_stop_l(mp->ffplayer);
        ffp_wait_stop_l(mp->ffplayer);
    }
    MPTRACE("airstashmp_shutdown_l()=void\n");
}

void airstashmp_shutdown(AirStashMediaPlayer *mp)
{
    return airstashmp_shutdown_l(mp);
}

void airstashmp_inc_ref(AirStashMediaPlayer *mp)
{
    assert(mp);
    __sync_fetch_and_add(&mp->ref_count, 1);
}

void airstashmp_dec_ref(AirStashMediaPlayer *mp)
{
    if (!mp)
        return;

    int ref_count = __sync_sub_and_fetch(&mp->ref_count, 1);
    if (ref_count == 0) {
        MPTRACE("airstashmp_dec_ref(): ref=0\n");
        airstashmp_shutdown(mp);
        airstashmp_destroy_p(&mp);
    }
}

void airstashmp_dec_ref_p(AirStashMediaPlayer **pmp)
{
    if (!pmp)
        return;

    airstashmp_dec_ref(*pmp);
    *pmp = NULL;
}

static int airstashmp_set_data_source_l(AirStashMediaPlayer *mp, const char *url)
{
    assert(mp);
    assert(url);

    // MPST_RET_IF_EQ(mp->mp_state, MP_STATE_IDLE);
    MPST_RET_IF_EQ(mp->mp_state, MP_STATE_INITIALIZED);
    MPST_RET_IF_EQ(mp->mp_state, MP_STATE_ASYNC_PREPARING);
    MPST_RET_IF_EQ(mp->mp_state, MP_STATE_PREPARED);
    MPST_RET_IF_EQ(mp->mp_state, MP_STATE_STARTED);
    MPST_RET_IF_EQ(mp->mp_state, MP_STATE_PAUSED);
    MPST_RET_IF_EQ(mp->mp_state, MP_STATE_COMPLETED);
    MPST_RET_IF_EQ(mp->mp_state, MP_STATE_STOPPED);
    MPST_RET_IF_EQ(mp->mp_state, MP_STATE_ERROR);
    MPST_RET_IF_EQ(mp->mp_state, MP_STATE_END);

    freep((void**)&mp->data_source);
    mp->data_source = strdup(url);
    if (!mp->data_source)
        return EAIRSTASH_OUT_OF_MEMORY;

    airstashmp_change_state_l(mp, MP_STATE_INITIALIZED);
    return 0;
}

int airstashmp_set_data_source(AirStashMediaPlayer *mp, const char *url)
{
    assert(mp);
    assert(url);
    MPTRACE("airstashmp_set_data_source(url=\"%s\")\n", url);
    pthread_mutex_lock(&mp->mutex);
    int retval = airstashmp_set_data_source_l(mp, url);
    pthread_mutex_unlock(&mp->mutex);
    MPTRACE("airstashmp_set_data_source(url=\"%s\")=%d\n", url, retval);
    return retval;
}

static int airstashmp_msg_loop(void *arg)
{
    AirStashMediaPlayer *mp = arg;
    int ret = mp->msg_loop(arg);
    return ret;
}

static int airstashmp_prepare_async_l(AirStashMediaPlayer *mp)
{
    assert(mp);

    MPST_RET_IF_EQ(mp->mp_state, MP_STATE_IDLE);
    // MPST_RET_IF_EQ(mp->mp_state, MP_STATE_INITIALIZED);
    MPST_RET_IF_EQ(mp->mp_state, MP_STATE_ASYNC_PREPARING);
    MPST_RET_IF_EQ(mp->mp_state, MP_STATE_PREPARED);
    MPST_RET_IF_EQ(mp->mp_state, MP_STATE_STARTED);
    MPST_RET_IF_EQ(mp->mp_state, MP_STATE_PAUSED);
    MPST_RET_IF_EQ(mp->mp_state, MP_STATE_COMPLETED);
    // MPST_RET_IF_EQ(mp->mp_state, MP_STATE_STOPPED);
    MPST_RET_IF_EQ(mp->mp_state, MP_STATE_ERROR);
    MPST_RET_IF_EQ(mp->mp_state, MP_STATE_END);

    assert(mp->data_source);

    airstashmp_change_state_l(mp, MP_STATE_ASYNC_PREPARING);

    msg_queue_start(&mp->ffplayer->msg_queue);

    // released in msg_loop
    airstashmp_inc_ref(mp);
    mp->msg_thread = SDL_CreateThreadEx(&mp->_msg_thread, airstashmp_msg_loop, mp, "ff_msg_loop");
    // msg_thread is detached inside msg_loop
    // TODO: 9 release weak_thiz if pthread_create() failed;

    int retval = ffp_prepare_async_l(mp->ffplayer, mp->data_source);
    if (retval < 0) {
        airstashmp_change_state_l(mp, MP_STATE_ERROR);
        return retval;
    }

    return 0;
}

int airstashmp_prepare_async(AirStashMediaPlayer *mp)
{
    assert(mp);
    MPTRACE("airstashmp_prepare_async()\n");
    pthread_mutex_lock(&mp->mutex);
    int retval = airstashmp_prepare_async_l(mp);
    pthread_mutex_unlock(&mp->mutex);
    MPTRACE("airstashmp_prepare_async()=%d\n", retval);
    return retval;
}

static int ikjmp_chkst_start_l(int mp_state)
{
    MPST_RET_IF_EQ(mp_state, MP_STATE_IDLE);
    MPST_RET_IF_EQ(mp_state, MP_STATE_INITIALIZED);
    MPST_RET_IF_EQ(mp_state, MP_STATE_ASYNC_PREPARING);
    // MPST_RET_IF_EQ(mp_state, MP_STATE_PREPARED);
    // MPST_RET_IF_EQ(mp_state, MP_STATE_STARTED);
    // MPST_RET_IF_EQ(mp_state, MP_STATE_PAUSED);
    // MPST_RET_IF_EQ(mp_state, MP_STATE_COMPLETED);
    MPST_RET_IF_EQ(mp_state, MP_STATE_STOPPED);
    MPST_RET_IF_EQ(mp_state, MP_STATE_ERROR);
    MPST_RET_IF_EQ(mp_state, MP_STATE_END);

    return 0;
}

static int airstashmp_start_l(AirStashMediaPlayer *mp)
{
    assert(mp);

    MP_RET_IF_FAILED(ikjmp_chkst_start_l(mp->mp_state));

    ffp_remove_msg(mp->ffplayer, FFP_REQ_START);
    ffp_remove_msg(mp->ffplayer, FFP_REQ_PAUSE);
    ffp_notify_msg1(mp->ffplayer, FFP_REQ_START);

    return 0;
}

int airstashmp_start(AirStashMediaPlayer *mp)
{
    assert(mp);
    MPTRACE("airstashmp_start()\n");
    pthread_mutex_lock(&mp->mutex);
    int retval = airstashmp_start_l(mp);
    pthread_mutex_unlock(&mp->mutex);
    MPTRACE("airstashmp_start()=%d\n", retval);
    return retval;
}

static int ikjmp_chkst_pause_l(int mp_state)
{
    MPST_RET_IF_EQ(mp_state, MP_STATE_IDLE);
    MPST_RET_IF_EQ(mp_state, MP_STATE_INITIALIZED);
    MPST_RET_IF_EQ(mp_state, MP_STATE_ASYNC_PREPARING);
    // MPST_RET_IF_EQ(mp_state, MP_STATE_PREPARED);
    // MPST_RET_IF_EQ(mp_state, MP_STATE_STARTED);
    // MPST_RET_IF_EQ(mp_state, MP_STATE_PAUSED);
    // MPST_RET_IF_EQ(mp_state, MP_STATE_COMPLETED);
    MPST_RET_IF_EQ(mp_state, MP_STATE_STOPPED);
    MPST_RET_IF_EQ(mp_state, MP_STATE_ERROR);
    MPST_RET_IF_EQ(mp_state, MP_STATE_END);

    return 0;
}

static int airstashmp_pause_l(AirStashMediaPlayer *mp)
{
    assert(mp);

    MP_RET_IF_FAILED(ikjmp_chkst_pause_l(mp->mp_state));

    ffp_remove_msg(mp->ffplayer, FFP_REQ_START);
    ffp_remove_msg(mp->ffplayer, FFP_REQ_PAUSE);
    ffp_notify_msg1(mp->ffplayer, FFP_REQ_PAUSE);

    return 0;
}

int airstashmp_pause(AirStashMediaPlayer *mp)
{
    assert(mp);
    MPTRACE("airstashmp_pause()\n");
    pthread_mutex_lock(&mp->mutex);
    int retval = airstashmp_pause_l(mp);
    pthread_mutex_unlock(&mp->mutex);
    MPTRACE("airstashmp_pause()=%d\n", retval);
    return retval;
}

static int airstashmp_stop_l(AirStashMediaPlayer *mp)
{
    assert(mp);

    MPST_RET_IF_EQ(mp->mp_state, MP_STATE_IDLE);
    MPST_RET_IF_EQ(mp->mp_state, MP_STATE_INITIALIZED);
    // MPST_RET_IF_EQ(mp->mp_state, MP_STATE_ASYNC_PREPARING);
    // MPST_RET_IF_EQ(mp->mp_state, MP_STATE_PREPARED);
    // MPST_RET_IF_EQ(mp->mp_state, MP_STATE_STARTED);
    // MPST_RET_IF_EQ(mp->mp_state, MP_STATE_PAUSED);
    // MPST_RET_IF_EQ(mp->mp_state, MP_STATE_COMPLETED);
    // MPST_RET_IF_EQ(mp->mp_state, MP_STATE_STOPPED);
    MPST_RET_IF_EQ(mp->mp_state, MP_STATE_ERROR);
    MPST_RET_IF_EQ(mp->mp_state, MP_STATE_END);

    ffp_remove_msg(mp->ffplayer, FFP_REQ_START);
    ffp_remove_msg(mp->ffplayer, FFP_REQ_PAUSE);
    int retval = ffp_stop_l(mp->ffplayer);
    if (retval < 0) {
        return retval;
    }

    airstashmp_change_state_l(mp, MP_STATE_STOPPED);
    return 0;
}

int airstashmp_stop(AirStashMediaPlayer *mp)
{
    assert(mp);
    MPTRACE("airstashmp_stop()\n");
    pthread_mutex_lock(&mp->mutex);
    int retval = airstashmp_stop_l(mp);
    pthread_mutex_unlock(&mp->mutex);
    MPTRACE("airstashmp_stop()=%d\n", retval);
    return retval;
}

bool airstashmp_is_playing(AirStashMediaPlayer *mp)
{
    assert(mp);
    if (mp->mp_state == MP_STATE_PREPARED ||
        mp->mp_state == MP_STATE_STARTED) {
        return true;
    }

    return false;
}

static int ikjmp_chkst_seek_l(int mp_state)
{
    MPST_RET_IF_EQ(mp_state, MP_STATE_IDLE);
    MPST_RET_IF_EQ(mp_state, MP_STATE_INITIALIZED);
    MPST_RET_IF_EQ(mp_state, MP_STATE_ASYNC_PREPARING);
    // MPST_RET_IF_EQ(mp_state, MP_STATE_PREPARED);
    // MPST_RET_IF_EQ(mp_state, MP_STATE_STARTED);
    // MPST_RET_IF_EQ(mp_state, MP_STATE_PAUSED);
    // MPST_RET_IF_EQ(mp_state, MP_STATE_COMPLETED);
    MPST_RET_IF_EQ(mp_state, MP_STATE_STOPPED);
    MPST_RET_IF_EQ(mp_state, MP_STATE_ERROR);
    MPST_RET_IF_EQ(mp_state, MP_STATE_END);

    return 0;
}

int airstashmp_seek_to_l(AirStashMediaPlayer *mp, long msec)
{
    assert(mp);

    MP_RET_IF_FAILED(ikjmp_chkst_seek_l(mp->mp_state));

    mp->seek_req = 1;
    mp->seek_msec = msec;
    ffp_remove_msg(mp->ffplayer, FFP_REQ_SEEK);
    ffp_notify_msg2(mp->ffplayer, FFP_REQ_SEEK, (int)msec);
    // TODO: 9 64-bit long?

    return 0;
}

int airstashmp_seek_to(AirStashMediaPlayer *mp, long msec)
{
    assert(mp);
    MPTRACE("airstashmp_seek_to(%ld)\n", msec);
    pthread_mutex_lock(&mp->mutex);
    int retval = airstashmp_seek_to_l(mp, msec);
    pthread_mutex_unlock(&mp->mutex);
    MPTRACE("airstashmp_seek_to(%ld)=%d\n", msec, retval);

    return retval;
}

int airstashmp_get_state(AirStashMediaPlayer *mp)
{
    return mp->mp_state;
}

static long airstashmp_get_current_position_l(AirStashMediaPlayer *mp)
{
    if (mp->seek_req)
        return mp->seek_msec;
    return ffp_get_current_position_l(mp->ffplayer);
}

long airstashmp_get_current_position(AirStashMediaPlayer *mp)
{
    assert(mp);
    pthread_mutex_lock(&mp->mutex);
    long retval;
    if (mp->seek_req)
        retval = mp->seek_msec;
    else
        retval = airstashmp_get_current_position_l(mp);
    pthread_mutex_unlock(&mp->mutex);
    return retval;
}

static long airstashmp_get_duration_l(AirStashMediaPlayer *mp)
{
    return ffp_get_duration_l(mp->ffplayer);
}

long airstashmp_get_duration(AirStashMediaPlayer *mp)
{
    assert(mp);
    pthread_mutex_lock(&mp->mutex);
    long retval = airstashmp_get_duration_l(mp);
    pthread_mutex_unlock(&mp->mutex);
    return retval;
}

static long airstashmp_get_playable_duration_l(AirStashMediaPlayer *mp)
{
    return ffp_get_playable_duration_l(mp->ffplayer);
}

long airstashmp_get_playable_duration(AirStashMediaPlayer *mp)
{
    assert(mp);
    pthread_mutex_lock(&mp->mutex);
    long retval = airstashmp_get_playable_duration_l(mp);
    pthread_mutex_unlock(&mp->mutex);
    return retval;
}

void airstashmp_set_loop(AirStashMediaPlayer *mp, int loop)
{
    assert(mp);
    pthread_mutex_lock(&mp->mutex);
    ffp_set_loop(mp->ffplayer, loop);
    pthread_mutex_unlock(&mp->mutex);
}

int airstashmp_get_loop(AirStashMediaPlayer *mp)
{
    assert(mp);
    pthread_mutex_lock(&mp->mutex);
    int loop = ffp_get_loop(mp->ffplayer);
    pthread_mutex_unlock(&mp->mutex);
    return loop;
}

void *airstashmp_get_weak_thiz(AirStashMediaPlayer *mp)
{
    return mp->weak_thiz;
}

void *airstashmp_set_weak_thiz(AirStashMediaPlayer *mp, void *weak_thiz)
{
    void *prev_weak_thiz = mp->weak_thiz;

    mp->weak_thiz = weak_thiz;

    return prev_weak_thiz;
}

int airstashmp_get_msg(AirStashMediaPlayer *mp, AVMessage *msg, int block)
{
    assert(mp);
    while (1) {
        int continue_wait_next_msg = 0;
        int retval = msg_queue_get(&mp->ffplayer->msg_queue, msg, block);
        if (retval <= 0)
            return retval;

        switch (msg->what) {
        case FFP_MSG_PREPARED:
            MPTRACE("airstashmp_get_msg: FFP_MSG_PREPARED\n");
            pthread_mutex_lock(&mp->mutex);
            if (mp->mp_state == MP_STATE_ASYNC_PREPARING) {
                airstashmp_change_state_l(mp, MP_STATE_PREPARED);
            } else {
                // FIXME: 1: onError() ?
                av_log(mp->ffplayer, AV_LOG_DEBUG, "FFP_MSG_PREPARED: expecting mp_state==MP_STATE_ASYNC_PREPARING\n");
            }
            if (ffp_is_paused_l(mp->ffplayer)) {
                airstashmp_change_state_l(mp, MP_STATE_PAUSED);
            }
            pthread_mutex_unlock(&mp->mutex);
            break;

        case FFP_MSG_COMPLETED:
            MPTRACE("airstashmp_get_msg: FFP_MSG_COMPLETED\n");

            pthread_mutex_lock(&mp->mutex);
            mp->restart = 1;
            mp->restart_from_beginning = 1;
            airstashmp_change_state_l(mp, MP_STATE_COMPLETED);
            pthread_mutex_unlock(&mp->mutex);
            break;

        case FFP_MSG_SEEK_COMPLETE:
            MPTRACE("airstashmp_get_msg: FFP_MSG_SEEK_COMPLETE\n");

            pthread_mutex_lock(&mp->mutex);
            mp->seek_req = 0;
            mp->seek_msec = 0;
            pthread_mutex_unlock(&mp->mutex);
            break;

        case FFP_REQ_START:
            MPTRACE("airstashmp_get_msg: FFP_REQ_START\n");
            continue_wait_next_msg = 1;
            pthread_mutex_lock(&mp->mutex);
            if (0 == ikjmp_chkst_start_l(mp->mp_state)) {
                // FIXME: 8 check seekable
                if (mp->restart) {
                    if (mp->restart_from_beginning) {
                        av_log(mp->ffplayer, AV_LOG_DEBUG, "airstashmp_get_msg: FFP_REQ_START: restart from beginning\n");
                        retval = ffp_start_from_l(mp->ffplayer, 0);
                        if (retval == 0)
                            airstashmp_change_state_l(mp, MP_STATE_STARTED);
                    } else {
                        av_log(mp->ffplayer, AV_LOG_DEBUG, "airstashmp_get_msg: FFP_REQ_START: restart from seek pos\n");
                        retval = ffp_start_l(mp->ffplayer);
                        if (retval == 0)
                            airstashmp_change_state_l(mp, MP_STATE_STARTED);
                    }
                    mp->restart = 0;
                    mp->restart_from_beginning = 0;
                } else {
                    av_log(mp->ffplayer, AV_LOG_DEBUG, "airstashmp_get_msg: FFP_REQ_START: start on fly\n");
                    retval = ffp_start_l(mp->ffplayer);
                    if (retval == 0)
                        airstashmp_change_state_l(mp, MP_STATE_STARTED);
                }
            }
            pthread_mutex_unlock(&mp->mutex);
            break;

        case FFP_REQ_PAUSE:
            MPTRACE("airstashmp_get_msg: FFP_REQ_PAUSE\n");
            continue_wait_next_msg = 1;
            pthread_mutex_lock(&mp->mutex);
            if (0 == ikjmp_chkst_pause_l(mp->mp_state)) {
                int pause_ret = ffp_pause_l(mp->ffplayer);
                if (pause_ret == 0)
                    airstashmp_change_state_l(mp, MP_STATE_PAUSED);
            }
            pthread_mutex_unlock(&mp->mutex);
            break;

        case FFP_REQ_SEEK:
            MPTRACE("airstashmp_get_msg: FFP_REQ_SEEK\n");
            continue_wait_next_msg = 1;

            pthread_mutex_lock(&mp->mutex);
            if (0 == ikjmp_chkst_seek_l(mp->mp_state)) {
                mp->restart_from_beginning = 0;
                if (0 == ffp_seek_to_l(mp->ffplayer, msg->arg1)) {
                    av_log(mp->ffplayer, AV_LOG_DEBUG, "airstashmp_get_msg: FFP_REQ_SEEK: seek to %d\n", (int)msg->arg1);
                }
            }
            pthread_mutex_unlock(&mp->mutex);
            break;
        }

        if (continue_wait_next_msg)
            continue;

        return retval;
    }

    return -1;
}
