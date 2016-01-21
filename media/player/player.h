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

#ifndef AIRSTASHPLAYER_ANDROID__AIRSTASHPLAYER_H
#define AIRSTASHPLAYER_ANDROID__AIRSTASHPLAYER_H

#include <stdbool.h>
#include "ff_ffmsg_queue.h"

#include "meta.h"

#ifndef MPTRACE
#define MPTRACE ALOGD
#endif

typedef struct AirStashMediaPlayer AirStashMediaPlayer;
struct FFPlayer;
struct SDL_Vout;

/*-
 MPST_CHECK_NOT_RET(mp->mp_state, MP_STATE_IDLE);
 MPST_CHECK_NOT_RET(mp->mp_state, MP_STATE_INITIALIZED);
 MPST_CHECK_NOT_RET(mp->mp_state, MP_STATE_ASYNC_PREPARING);
 MPST_CHECK_NOT_RET(mp->mp_state, MP_STATE_PREPARED);
 MPST_CHECK_NOT_RET(mp->mp_state, MP_STATE_STARTED);
 MPST_CHECK_NOT_RET(mp->mp_state, MP_STATE_PAUSED);
 MPST_CHECK_NOT_RET(mp->mp_state, MP_STATE_COMPLETED);
 MPST_CHECK_NOT_RET(mp->mp_state, MP_STATE_STOPPED);
 MPST_CHECK_NOT_RET(mp->mp_state, MP_STATE_ERROR);
 MPST_CHECK_NOT_RET(mp->mp_state, MP_STATE_END);
 */

/*-
 * airstashmp_set_data_source()  -> MP_STATE_INITIALIZED
 *
 * airstashmp_reset              -> self
 * airstashmp_release            -> MP_STATE_END
 */
#define MP_STATE_IDLE               0

/*-
 * airstashmp_prepare_async()    -> MP_STATE_ASYNC_PREPARING
 *
 * airstashmp_reset              -> MP_STATE_IDLE
 * airstashmp_release            -> MP_STATE_END
 */
#define MP_STATE_INITIALIZED        1

/*-
 *                   ...    -> MP_STATE_PREPARED
 *                   ...    -> MP_STATE_ERROR
 *
 * airstashmp_reset              -> MP_STATE_IDLE
 * airstashmp_release            -> MP_STATE_END
 */
#define MP_STATE_ASYNC_PREPARING    2

/*-
 * airstashmp_seek_to()          -> self
 * airstashmp_start()            -> MP_STATE_STARTED
 *
 * airstashmp_reset              -> MP_STATE_IDLE
 * airstashmp_release            -> MP_STATE_END
 */
#define MP_STATE_PREPARED           3

/*-
 * airstashmp_seek_to()          -> self
 * airstashmp_start()            -> self
 * airstashmp_pause()            -> MP_STATE_PAUSED
 * airstashmp_stop()             -> MP_STATE_STOPPED
 *                   ...    -> MP_STATE_COMPLETED
 *                   ...    -> MP_STATE_ERROR
 *
 * airstashmp_reset              -> MP_STATE_IDLE
 * airstashmp_release            -> MP_STATE_END
 */
#define MP_STATE_STARTED            4

/*-
 * airstashmp_seek_to()          -> self
 * airstashmp_start()            -> MP_STATE_STARTED
 * airstashmp_pause()            -> self
 * airstashmp_stop()             -> MP_STATE_STOPPED
 *
 * airstashmp_reset              -> MP_STATE_IDLE
 * airstashmp_release            -> MP_STATE_END
 */
#define MP_STATE_PAUSED             5

/*-
 * airstashmp_seek_to()          -> self
 * airstashmp_start()            -> MP_STATE_STARTED (from beginning)
 * airstashmp_pause()            -> self
 * airstashmp_stop()             -> MP_STATE_STOPPED
 *
 * airstashmp_reset              -> MP_STATE_IDLE
 * airstashmp_release            -> MP_STATE_END
 */
#define MP_STATE_COMPLETED          6

/*-
 * airstashmp_stop()             -> self
 * airstashmp_prepare_async()    -> MP_STATE_ASYNC_PREPARING
 *
 * airstashmp_reset              -> MP_STATE_IDLE
 * airstashmp_release            -> MP_STATE_END
 */
#define MP_STATE_STOPPED            7

/*-
 * airstashmp_reset              -> MP_STATE_IDLE
 * airstashmp_release            -> MP_STATE_END
 */
#define MP_STATE_ERROR              8

/*-
 * airstashmp_release            -> self
 */
#define MP_STATE_END                9



#define AIRSTASHMP_IO_STAT_READ 1


#define AIRSTASHMP_OPT_CATEGORY_FORMAT FFP_OPT_CATEGORY_FORMAT
#define AIRSTASHMP_OPT_CATEGORY_CODEC  FFP_OPT_CATEGORY_CODEC
#define AIRSTASHMP_OPT_CATEGORY_SWS    FFP_OPT_CATEGORY_SWS
#define AIRSTASHMP_OPT_CATEGORY_PLAYER FFP_OPT_CATEGORY_PLAYER


void            airstashmp_global_init();
void            airstashmp_global_uninit();
void            airstashmp_global_set_log_report(int use_report);
void            airstashmp_global_set_log_level(int log_level);   // log_level = AV_LOG_xxx
void            airstashmp_global_set_inject_callback(airstash_inject_callback cb);
const char     *airstashmp_version_ident();
unsigned int    airstashmp_version_int();
void            airstashmp_io_stat_register(void (*cb)(const char *url, int type, int bytes));
void            airstashmp_io_stat_complete_register(void (*cb)(const char *url,
                                                           int64_t read_bytes, int64_t total_size,
                                                           int64_t elpased_time, int64_t total_duration));

// ref_count is 1 after open
AirStashMediaPlayer *airstashmp_create(int (*msg_loop)(void*));
void            airstashmp_set_inject_opaque(AirStashMediaPlayer *mp, void *opaque);

void            airstashmp_set_option(AirStashMediaPlayer *mp, int opt_category, const char *name, const char *value);
void            airstashmp_set_option_int(AirStashMediaPlayer *mp, int opt_category, const char *name, int64_t value);

int             airstashmp_get_video_codec_info(AirStashMediaPlayer *mp, char **codec_info);
int             airstashmp_get_audio_codec_info(AirStashMediaPlayer *mp, char **codec_info);
void            airstashmp_set_playback_rate(AirStashMediaPlayer *mp, float rate);
int             airstashmp_set_stream_selected(AirStashMediaPlayer *mp, int stream, int selected);

float           airstashmp_get_property_float(AirStashMediaPlayer *mp, int id, float default_value);
void            airstashmp_set_property_float(AirStashMediaPlayer *mp, int id, float value);
int64_t         airstashmp_get_property_int64(AirStashMediaPlayer *mp, int id, int64_t default_value);
void            airstashmp_set_property_int64(AirStashMediaPlayer *mp, int id, int64_t value);

// must be freed with free();
AirStashMediaMeta   *airstashmp_get_meta_l(AirStashMediaPlayer *mp);

// preferred to be called explicity, can be called multiple times
// NOTE: airstashmp_shutdown may block thread
void            airstashmp_shutdown(AirStashMediaPlayer *mp);

void            airstashmp_inc_ref(AirStashMediaPlayer *mp);

// call close at last release, also free memory
// NOTE: airstashmp_dec_ref may block thread
void            airstashmp_dec_ref(AirStashMediaPlayer *mp);
void            airstashmp_dec_ref_p(AirStashMediaPlayer **pmp);

int             airstashmp_set_data_source(AirStashMediaPlayer *mp, const char *url);
int             airstashmp_prepare_async(AirStashMediaPlayer *mp);
int             airstashmp_start(AirStashMediaPlayer *mp);
int             airstashmp_pause(AirStashMediaPlayer *mp);
int             airstashmp_stop(AirStashMediaPlayer *mp);
int             airstashmp_seek_to(AirStashMediaPlayer *mp, long msec);
int             airstashmp_get_state(AirStashMediaPlayer *mp);
bool            airstashmp_is_playing(AirStashMediaPlayer *mp);
long            airstashmp_get_current_position(AirStashMediaPlayer *mp);
long            airstashmp_get_duration(AirStashMediaPlayer *mp);
long            airstashmp_get_playable_duration(AirStashMediaPlayer *mp);
void            airstashmp_set_loop(AirStashMediaPlayer *mp, int loop);
int             airstashmp_get_loop(AirStashMediaPlayer *mp);

void           *airstashmp_get_weak_thiz(AirStashMediaPlayer *mp);
void           *airstashmp_set_weak_thiz(AirStashMediaPlayer *mp, void *weak_thiz);

/* return < 0 if aborted, 0 if no packet and > 0 if packet.  */
int             airstashmp_get_msg(AirStashMediaPlayer *mp, AVMessage *msg, int block);

#endif
