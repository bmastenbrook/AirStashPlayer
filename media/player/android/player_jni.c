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

#include <assert.h>
#include <string.h>
#include <pthread.h>
#include <jni.h>
#include <unistd.h>
#include "sdl/sdl_log.h"
#include "sdl/android/airstash/c/java/util/ArrayList.h"
#include "sdl/android/airstash/c/android/os/Bundle.h"
#include "sdl/android/airstash/c/com/airstash/player/player/AirStashMediaPlayer.h"
#include "sdl/android/airstash/c/com/airstash/player/player/misc/IMediaDataSource.h"
#include "../ff_ffplay.h"
#include "ffmpeg_api_jni.h"
#include "player_android_def.h"
#include "player_android.h"
#include "sdl/android/sdl_android_jni.h"
#include "sdl/android/sdl_codec_android_mediadef.h"
#include "player/avformat/avformat.h"

#define JNI_MODULE_PACKAGE      "com/AirStash/player/player"
#define JNI_CLASS_AIRSTASHPLAYER     "com/AirStash/player/player/AirStashMediaPlayer"
#define JNI_AIRSTASH_MEDIA_EXCEPTION "com/AirStash/player/player/AirStashMediaException"

#define AIRSTASH_CHECK_MPRET_GOTO(retval, env, label) \
    JNI_CHECK_GOTO((retval != EAIRSTASH_INVALID_STATE), env, "java/lang/IllegalStateException", NULL, label); \
    JNI_CHECK_GOTO((retval != EAIRSTASH_OUT_OF_MEMORY), env, "java/lang/OutOfMemoryError", NULL, label); \
    JNI_CHECK_GOTO((retval == 0), env, JNI_AIRSTASH_MEDIA_EXCEPTION, NULL, label);

static JavaVM* g_jvm;

typedef struct player_fields_t {
    pthread_mutex_t mutex;
    jclass clazz;
} player_fields_t;
static player_fields_t g_clazz;

static int inject_callback(void *opaque, int type, void *data, size_t data_size);
static bool mediacodec_select_callback(void *opaque, airstashmp_mediacodecinfo_context *mcc);

static AirStashMediaPlayer *jni_get_media_player(JNIEnv* env, jobject thiz)
{
    pthread_mutex_lock(&g_clazz.mutex);

    AirStashMediaPlayer *mp = (AirStashMediaPlayer *) (intptr_t) AirStashC_AirStashMediaPlayer__mNativeMediaPlayer__get__catchAll(env, thiz);
    if (mp) {
        airstashmp_inc_ref(mp);
    }

    pthread_mutex_unlock(&g_clazz.mutex);
    return mp;
}

static AirStashMediaPlayer *jni_set_media_player(JNIEnv* env, jobject thiz, AirStashMediaPlayer *mp)
{
    pthread_mutex_lock(&g_clazz.mutex);

    AirStashMediaPlayer *old = (AirStashMediaPlayer*) (intptr_t) AirStashC_AirStashMediaPlayer__mNativeMediaPlayer__get__catchAll(env, thiz);
    if (mp) {
        airstashmp_inc_ref(mp);
    }
    AirStashC_AirStashMediaPlayer__mNativeMediaPlayer__set__catchAll(env, thiz, (intptr_t) mp);

    pthread_mutex_unlock(&g_clazz.mutex);

    // NOTE: airstashmp_dec_ref may block thread
    if (old != NULL ) {
        airstashmp_dec_ref_p(&old);
    }

    return old;
}

static int64_t jni_set_media_data_source(JNIEnv* env, jobject thiz, jobject media_data_source)
{
    int64_t nativeMediaDataSource = 0;

    pthread_mutex_lock(&g_clazz.mutex);

    jobject old = (jobject) (intptr_t) AirStashC_AirStashMediaPlayer__mNativeMediaDataSource__get__catchAll(env, thiz);
    if (old) {
        AirStashC_IMediaDataSource__close__catchAll(env, old);
        AirStash_DeleteGlobalRef__p(env, &old);
        AirStashC_AirStashMediaPlayer__mNativeMediaDataSource__set__catchAll(env, thiz, 0);
    }

    if (media_data_source) {
        jobject global_media_data_source = (*env)->NewGlobalRef(env, media_data_source);
        if (AirStash_ExceptionCheck__catchAll(env) || !global_media_data_source)
            goto fail;

        nativeMediaDataSource = (int64_t) (intptr_t) global_media_data_source;
        AirStashC_AirStashMediaPlayer__mNativeMediaDataSource__set__catchAll(env, thiz, (jlong) nativeMediaDataSource);
    }

fail:
    pthread_mutex_unlock(&g_clazz.mutex);
    return nativeMediaDataSource;
}

static int message_loop(void *arg);

static void
AirStashMediaPlayer_setDataSourceAndHeaders(
    JNIEnv *env, jobject thiz, jstring path,
    jobjectArray keys, jobjectArray values)
{
    MPTRACE("%s\n", __func__);
    int retval = 0;
    const char *c_path = NULL;
    AirStashMediaPlayer *mp = jni_get_media_player(env, thiz);
    JNI_CHECK_GOTO(path, env, "java/lang/IllegalArgumentException", "mpjni: setDataSource: null path", LABEL_RETURN);
    JNI_CHECK_GOTO(mp, env, "java/lang/IllegalStateException", "mpjni: setDataSource: null mp", LABEL_RETURN);

    c_path = (*env)->GetStringUTFChars(env, path, NULL );
    JNI_CHECK_GOTO(c_path, env, "java/lang/OutOfMemoryError", "mpjni: setDataSource: path.string oom", LABEL_RETURN);

    ALOGV("setDataSource: path %s", c_path);
    retval = airstashmp_set_data_source(mp, c_path);
    (*env)->ReleaseStringUTFChars(env, path, c_path);

    AIRSTASH_CHECK_MPRET_GOTO(retval, env, LABEL_RETURN);

LABEL_RETURN:
    airstashmp_dec_ref_p(&mp);
}

static void
AirStashMediaPlayer_setDataSourceFd(JNIEnv *env, jobject thiz, jint fd)
{
    MPTRACE("%s\n", __func__);
    int retval = 0;
    int dupFd = 0;
    char uri[128];
    AirStashMediaPlayer *mp = jni_get_media_player(env, thiz);
    JNI_CHECK_GOTO(fd > 0, env, "java/lang/IllegalArgumentException", "mpjni: setDataSourceFd: null fd", LABEL_RETURN);
    JNI_CHECK_GOTO(mp, env, "java/lang/IllegalStateException", "mpjni: setDataSourceFd: null mp", LABEL_RETURN);

    dupFd = dup(fd);

    ALOGV("setDataSourceFd: dup(%d)=%d\n", fd, dupFd);
    snprintf(uri, sizeof(uri), "pipe:%d", dupFd);
    retval = airstashmp_set_data_source(mp, uri);

    AIRSTASH_CHECK_MPRET_GOTO(retval, env, LABEL_RETURN);

LABEL_RETURN:
    airstashmp_dec_ref_p(&mp);
}

static void
AirStashMediaPlayer_setDataSourceCallback(JNIEnv *env, jobject thiz, jobject callback)
{
    MPTRACE("%s\n", __func__);
    int retval = 0;
    char uri[128];
    int64_t nativeMediaDataSource = 0;
    AirStashMediaPlayer *mp = jni_get_media_player(env, thiz);
    JNI_CHECK_GOTO(callback, env, "java/lang/IllegalArgumentException", "mpjni: setDataSourceCallback: null fd", LABEL_RETURN);
    JNI_CHECK_GOTO(mp, env, "java/lang/IllegalStateException", "mpjni: setDataSourceCallback: null mp", LABEL_RETURN);

    nativeMediaDataSource = jni_set_media_data_source(env, thiz, callback);
    JNI_CHECK_GOTO(nativeMediaDataSource, env, "java/lang/IllegalStateException", "mpjni: jni_set_media_data_source: NewGlobalRef", LABEL_RETURN);

    ALOGV("setDataSourceCallback: %"PRId64"\n", nativeMediaDataSource);
    snprintf(uri, sizeof(uri), "airstashmediadatasource:%"PRId64, nativeMediaDataSource);

    retval = airstashmp_set_data_source(mp, uri);

    AIRSTASH_CHECK_MPRET_GOTO(retval, env, LABEL_RETURN);

LABEL_RETURN:
    airstashmp_dec_ref_p(&mp);
}

static void
AirStashMediaPlayer_setVideoSurface(JNIEnv *env, jobject thiz, jobject jsurface)
{
    MPTRACE("%s\n", __func__);
    AirStashMediaPlayer *mp = jni_get_media_player(env, thiz);
    JNI_CHECK_GOTO(mp, env, NULL, "mpjni: setVideoSurface: null mp", LABEL_RETURN);

    airstashmp_android_set_surface(env, mp, jsurface);

LABEL_RETURN:
    airstashmp_dec_ref_p(&mp);
    return;
}

static void
AirStashMediaPlayer_prepareAsync(JNIEnv *env, jobject thiz)
{
    MPTRACE("%s\n", __func__);
    int retval = 0;
    AirStashMediaPlayer *mp = jni_get_media_player(env, thiz);
    JNI_CHECK_GOTO(mp, env, "java/lang/IllegalStateException", "mpjni: prepareAsync: null mp", LABEL_RETURN);

    retval = airstashmp_prepare_async(mp);
    AIRSTASH_CHECK_MPRET_GOTO(retval, env, LABEL_RETURN);

LABEL_RETURN:
    airstashmp_dec_ref_p(&mp);
}

static void
AirStashMediaPlayer_start(JNIEnv *env, jobject thiz)
{
    MPTRACE("%s\n", __func__);
    AirStashMediaPlayer *mp = jni_get_media_player(env, thiz);
    JNI_CHECK_GOTO(mp, env, "java/lang/IllegalStateException", "mpjni: start: null mp", LABEL_RETURN);

    airstashmp_start(mp);

LABEL_RETURN:
    airstashmp_dec_ref_p(&mp);
}

static void
AirStashMediaPlayer_stop(JNIEnv *env, jobject thiz)
{
    AirStashMediaPlayer *mp = jni_get_media_player(env, thiz);
    JNI_CHECK_GOTO(mp, env, "java/lang/IllegalStateException", "mpjni: stop: null mp", LABEL_RETURN);

    airstashmp_stop(mp);

LABEL_RETURN:
    airstashmp_dec_ref_p(&mp);
}

static void
AirStashMediaPlayer_pause(JNIEnv *env, jobject thiz)
{
    AirStashMediaPlayer *mp = jni_get_media_player(env, thiz);
    JNI_CHECK_GOTO(mp, env, "java/lang/IllegalStateException", "mpjni: pause: null mp", LABEL_RETURN);

    airstashmp_pause(mp);

LABEL_RETURN:
    airstashmp_dec_ref_p(&mp);
}

static void
AirStashMediaPlayer_seekTo(JNIEnv *env, jobject thiz, jlong msec)
{
    MPTRACE("%s\n", __func__);
    AirStashMediaPlayer *mp = jni_get_media_player(env, thiz);
    JNI_CHECK_GOTO(mp, env, "java/lang/IllegalStateException", "mpjni: seekTo: null mp", LABEL_RETURN);

    airstashmp_seek_to(mp, msec);

LABEL_RETURN:
    airstashmp_dec_ref_p(&mp);
}

static jboolean
AirStashMediaPlayer_isPlaying(JNIEnv *env, jobject thiz)
{
    jboolean retval = JNI_FALSE;
    AirStashMediaPlayer *mp = jni_get_media_player(env, thiz);
    JNI_CHECK_GOTO(mp, env, NULL, "mpjni: isPlaying: null mp", LABEL_RETURN);

    retval = airstashmp_is_playing(mp) ? JNI_TRUE : JNI_FALSE;

LABEL_RETURN:
    airstashmp_dec_ref_p(&mp);
    return retval;
}

static jlong
AirStashMediaPlayer_getCurrentPosition(JNIEnv *env, jobject thiz)
{
    jlong retval = 0;
    AirStashMediaPlayer *mp = jni_get_media_player(env, thiz);
    JNI_CHECK_GOTO(mp, env, NULL, "mpjni: getCurrentPosition: null mp", LABEL_RETURN);

    retval = airstashmp_get_current_position(mp);

LABEL_RETURN:
    airstashmp_dec_ref_p(&mp);
    return retval;
}

static jlong
AirStashMediaPlayer_getDuration(JNIEnv *env, jobject thiz)
{
    jlong retval = 0;
    AirStashMediaPlayer *mp = jni_get_media_player(env, thiz);
    JNI_CHECK_GOTO(mp, env, NULL, "mpjni: getDuration: null mp", LABEL_RETURN);

    retval = airstashmp_get_duration(mp);

LABEL_RETURN:
    airstashmp_dec_ref_p(&mp);
    return retval;
}

static void
AirStashMediaPlayer_release(JNIEnv *env, jobject thiz)
{
    MPTRACE("%s\n", __func__);
    AirStashMediaPlayer *mp = jni_get_media_player(env, thiz);
    if (!mp)
        return;

    airstashmp_android_set_surface(env, mp, NULL);
    // explicit shutdown mp, in case it is not the last mp-ref here
    airstashmp_shutdown(mp);
    //only delete weak_thiz at release
    jobject weak_thiz = (jobject) airstashmp_set_weak_thiz(mp, NULL );
    (*env)->DeleteGlobalRef(env, weak_thiz);
    jni_set_media_player(env, thiz, NULL);
    jni_set_media_data_source(env, thiz, NULL);

    airstashmp_dec_ref_p(&mp);
}

static void AirStashMediaPlayer_native_setup(JNIEnv *env, jobject thiz, jobject weak_this);
static void
AirStashMediaPlayer_reset(JNIEnv *env, jobject thiz)
{
    MPTRACE("%s\n", __func__);
    AirStashMediaPlayer *mp = jni_get_media_player(env, thiz);
    if (!mp)
        return;

    jobject weak_thiz = (jobject) airstashmp_set_weak_thiz(mp, NULL );

    AirStashMediaPlayer_release(env, thiz);
    AirStashMediaPlayer_native_setup(env, thiz, weak_thiz);

    airstashmp_dec_ref_p(&mp);
}

static void
AirStashMediaPlayer_setLoopCount(JNIEnv *env, jobject thiz, jint loop_count)
{
    MPTRACE("%s\n", __func__);
    AirStashMediaPlayer *mp = jni_get_media_player(env, thiz);
    JNI_CHECK_GOTO(mp, env, NULL, "mpjni: setLoopCount: null mp", LABEL_RETURN);

    airstashmp_set_loop(mp, loop_count);

LABEL_RETURN:
    airstashmp_dec_ref_p(&mp);
}

static jint
AirStashMediaPlayer_getLoopCount(JNIEnv *env, jobject thiz)
{
    jint loop_count = 1;
    MPTRACE("%s\n", __func__);
    AirStashMediaPlayer *mp = jni_get_media_player(env, thiz);
    JNI_CHECK_GOTO(mp, env, NULL, "mpjni: getLoopCount: null mp", LABEL_RETURN);

    loop_count = airstashmp_get_loop(mp);

LABEL_RETURN:
    airstashmp_dec_ref_p(&mp);
    return loop_count;
}

static jfloat
AirStashMediaPlayer_getPropertyFloat(JNIEnv *env, jobject thiz, jint id, jfloat default_value)
{
    jfloat value = default_value;
    AirStashMediaPlayer *mp = jni_get_media_player(env, thiz);
    JNI_CHECK_GOTO(mp, env, NULL, "mpjni: getPropertyFloat: null mp", LABEL_RETURN);

    value = airstashmp_get_property_float(mp, id, default_value);

LABEL_RETURN:
    airstashmp_dec_ref_p(&mp);
    return value;
}

static void
AirStashMediaPlayer_setPropertyFloat(JNIEnv *env, jobject thiz, jint id, jfloat value)
{
    AirStashMediaPlayer *mp = jni_get_media_player(env, thiz);
    JNI_CHECK_GOTO(mp, env, NULL, "mpjni: setPropertyFloat: null mp", LABEL_RETURN);

    airstashmp_set_property_float(mp, id, value);

LABEL_RETURN:
    airstashmp_dec_ref_p(&mp);
    return;
}

static jlong
AirStashMediaPlayer_getPropertyLong(JNIEnv *env, jobject thiz, jint id, jlong default_value)
{
    jlong value = default_value;
    AirStashMediaPlayer *mp = jni_get_media_player(env, thiz);
    JNI_CHECK_GOTO(mp, env, NULL, "mpjni: getPropertyLong: null mp", LABEL_RETURN);

    value = airstashmp_get_property_int64(mp, id, default_value);

LABEL_RETURN:
    airstashmp_dec_ref_p(&mp);
    return value;
}

static void
AirStashMediaPlayer_setPropertyLong(JNIEnv *env, jobject thiz, jint id, jlong value)
{
    AirStashMediaPlayer *mp = jni_get_media_player(env, thiz);
    JNI_CHECK_GOTO(mp, env, NULL, "mpjni: setPropertyLong: null mp", LABEL_RETURN);

    airstashmp_set_property_int64(mp, id, value);

LABEL_RETURN:
    airstashmp_dec_ref_p(&mp);
    return;
}

static void
AirStashMediaPlayer_setStreamSelected(JNIEnv *env, jobject thiz, jint stream, jboolean selected)
{
    AirStashMediaPlayer *mp = jni_get_media_player(env, thiz);
    int ret = 0;
    JNI_CHECK_GOTO(mp, env, NULL, "mpjni: setStreamSelected: null mp", LABEL_RETURN);

    ret = airstashmp_set_stream_selected(mp, stream, selected);
    if (ret < 0) {
        ALOGE("failed to %s %d", selected ? "select" : "deselect", stream);
        goto LABEL_RETURN;
    }

LABEL_RETURN:
    airstashmp_dec_ref_p(&mp);
    return;
}

static void
AirStashMediaPlayer_setVolume(JNIEnv *env, jobject thiz, jfloat leftVolume, jfloat rightVolume)
{
    MPTRACE("%s\n", __func__);
    AirStashMediaPlayer *mp = jni_get_media_player(env, thiz);
    JNI_CHECK_GOTO(mp, env, NULL, "mpjni: setVolume: null mp", LABEL_RETURN);

    airstashmp_android_set_volume(env, mp, leftVolume, rightVolume);

LABEL_RETURN:
    airstashmp_dec_ref_p(&mp);
}

static jint
AirStashMediaPlayer_getAudioSessionId(JNIEnv *env, jobject thiz)
{
    jint audio_session_id = 0;
    MPTRACE("%s\n", __func__);
    AirStashMediaPlayer *mp = jni_get_media_player(env, thiz);
    JNI_CHECK_GOTO(mp, env, NULL, "mpjni: getAudioSessionId: null mp", LABEL_RETURN);

    audio_session_id = airstashmp_android_get_audio_session_id(env, mp);

LABEL_RETURN:
    airstashmp_dec_ref_p(&mp);
    return audio_session_id;
}

static void
AirStashMediaPlayer_setOption(JNIEnv *env, jobject thiz, jint category, jobject name, jobject value)
{
    MPTRACE("%s\n", __func__);
    AirStashMediaPlayer *mp = jni_get_media_player(env, thiz);
    const char *c_name = NULL;
    const char *c_value = NULL;
    JNI_CHECK_GOTO(mp, env, "java/lang/IllegalStateException", "mpjni: setOption: null mp", LABEL_RETURN);

    c_name = (*env)->GetStringUTFChars(env, name, NULL );
    JNI_CHECK_GOTO(c_name, env, "java/lang/OutOfMemoryError", "mpjni: setOption: name.string oom", LABEL_RETURN);

    if (value) {
        c_value = (*env)->GetStringUTFChars(env, value, NULL );
        JNI_CHECK_GOTO(c_name, env, "java/lang/OutOfMemoryError", "mpjni: setOption: name.string oom", LABEL_RETURN);
    }

    airstashmp_set_option(mp, category, c_name, c_value);

LABEL_RETURN:
    if (c_name)
        (*env)->ReleaseStringUTFChars(env, name, c_name);
    if (c_value)
        (*env)->ReleaseStringUTFChars(env, value, c_value);
    airstashmp_dec_ref_p(&mp);
}

static void
AirStashMediaPlayer_setOptionLong(JNIEnv *env, jobject thiz, jint category, jobject name, jlong value)
{
    MPTRACE("%s\n", __func__);
    AirStashMediaPlayer *mp = jni_get_media_player(env, thiz);
    const char *c_name = NULL;
    JNI_CHECK_GOTO(mp, env, "java/lang/IllegalStateException", "mpjni: setOptionLong: null mp", LABEL_RETURN);

    c_name = (*env)->GetStringUTFChars(env, name, NULL );
    JNI_CHECK_GOTO(c_name, env, "java/lang/OutOfMemoryError", "mpjni: setOptionLong: name.string oom", LABEL_RETURN);

    airstashmp_set_option_int(mp, category, c_name, value);

LABEL_RETURN:
    if (c_name)
        (*env)->ReleaseStringUTFChars(env, name, c_name);
    airstashmp_dec_ref_p(&mp);
}

static jstring
AirStashMediaPlayer_getColorFormatName(JNIEnv *env, jclass clazz, jint mediaCodecColorFormat)
{
    const char *codec_name = SDL_AMediaCodec_getColorFormatName(mediaCodecColorFormat);
    if (!codec_name)
        return NULL ;

    return (*env)->NewStringUTF(env, codec_name);
}

static jstring
AirStashMediaPlayer_getVideoCodecInfo(JNIEnv *env, jobject thiz)
{
    MPTRACE("%s\n", __func__);
    jstring jcodec_info = NULL;
    int ret = 0;
    char *codec_info = NULL;
    AirStashMediaPlayer *mp = jni_get_media_player(env, thiz);
    JNI_CHECK_GOTO(mp, env, "java/lang/IllegalStateException", "mpjni: getVideoCodecInfo: null mp", LABEL_RETURN);

    ret = airstashmp_get_video_codec_info(mp, &codec_info);
    if (ret < 0 || !codec_info)
        goto LABEL_RETURN;

    jcodec_info = (*env)->NewStringUTF(env, codec_info);
LABEL_RETURN:
    if (codec_info)
        free(codec_info);

    airstashmp_dec_ref_p(&mp);
    return jcodec_info;
}

static jstring
AirStashMediaPlayer_getAudioCodecInfo(JNIEnv *env, jobject thiz)
{
    MPTRACE("%s\n", __func__);
    jstring jcodec_info = NULL;
    int ret = 0;
    char *codec_info = NULL;
    AirStashMediaPlayer *mp = jni_get_media_player(env, thiz);
    JNI_CHECK_GOTO(mp, env, "java/lang/IllegalStateException", "mpjni: getAudioCodecInfo: null mp", LABEL_RETURN);

    ret = airstashmp_get_audio_codec_info(mp, &codec_info);
    if (ret < 0 || !codec_info)
        goto LABEL_RETURN;

    jcodec_info = (*env)->NewStringUTF(env, codec_info);
LABEL_RETURN:
    if (codec_info)
        free(codec_info);

    airstashmp_dec_ref_p(&mp);
    return jcodec_info;
}

inline static void fillMetaInternal(JNIEnv *env, jobject jbundle, AirStashMediaMeta *meta, const char *key, const char *default_value)
{
    const char *value = airstashmeta_get_string_l(meta, key);
    if (value == NULL )
        value = default_value;

    AirStashC_Bundle__putString__withCString__catchAll(env, jbundle, key, value);
}

static jobject
AirStashMediaPlayer_getMediaMeta(JNIEnv *env, jobject thiz)
{
    MPTRACE("%s\n", __func__);
    bool is_locked = false;
    jobject jret_bundle = NULL;
    jobject jlocal_bundle = NULL;
    jobject jstream_bundle = NULL;
    jobject jarray_list = NULL;
    AirStashMediaMeta *meta = NULL;
    AirStashMediaPlayer *mp = jni_get_media_player(env, thiz);
    JNI_CHECK_GOTO(mp, env, "java/lang/IllegalStateException", "mpjni: getMediaMeta: null mp", LABEL_RETURN);

    meta = airstashmp_get_meta_l(mp);
    if (!meta)
        goto LABEL_RETURN;

    airstashmeta_lock(meta);
    is_locked = true;

    jlocal_bundle = AirStashC_Bundle__Bundle(env);
    if (AirStash_ExceptionCheck__throwAny(env)) {
        goto LABEL_RETURN;
    }

    fillMetaInternal(env, jlocal_bundle, meta, AIRSTASHM_KEY_FORMAT, NULL );
    fillMetaInternal(env, jlocal_bundle, meta, AIRSTASHM_KEY_DURATION_US, NULL );
    fillMetaInternal(env, jlocal_bundle, meta, AIRSTASHM_KEY_START_US, NULL );
    fillMetaInternal(env, jlocal_bundle, meta, AIRSTASHM_KEY_START_US, NULL );

    fillMetaInternal(env, jlocal_bundle, meta, AIRSTASHM_KEY_VIDEO_STREAM, "-1");
    fillMetaInternal(env, jlocal_bundle, meta, AIRSTASHM_KEY_AUDIO_STREAM, "-1");

    jarray_list = AirStashC_ArrayList__ArrayList(env);
    if (AirStash_ExceptionCheck__throwAny(env)) {
        goto LABEL_RETURN;
    }

    size_t count = airstashmeta_get_children_count_l(meta);
    for (size_t i = 0; i < count; ++i) {
        AirStashMediaMeta *streamRawMeta = airstashmeta_get_child_l(meta, i);
        if (streamRawMeta) {
            jstream_bundle = AirStashC_Bundle__Bundle(env);
            if (AirStash_ExceptionCheck__throwAny(env)) {
                goto LABEL_RETURN;
            }

            fillMetaInternal(env, jstream_bundle, streamRawMeta, AIRSTASHM_KEY_TYPE,     AIRSTASHM_VAL_TYPE__UNKNOWN);
            fillMetaInternal(env, jstream_bundle, streamRawMeta, AIRSTASHM_KEY_LANGUAGE, NULL);
            const char *type = airstashmeta_get_string_l(streamRawMeta, AIRSTASHM_KEY_TYPE);
            if (type) {
                fillMetaInternal(env, jstream_bundle, streamRawMeta, AIRSTASHM_KEY_CODEC_NAME, NULL );
                fillMetaInternal(env, jstream_bundle, streamRawMeta, AIRSTASHM_KEY_CODEC_PROFILE, NULL );
                fillMetaInternal(env, jstream_bundle, streamRawMeta, AIRSTASHM_KEY_CODEC_LEVEL, NULL );
                fillMetaInternal(env, jstream_bundle, streamRawMeta, AIRSTASHM_KEY_CODEC_LONG_NAME, NULL );
                fillMetaInternal(env, jstream_bundle, streamRawMeta, AIRSTASHM_KEY_CODEC_PIXEL_FORMAT, NULL );
                fillMetaInternal(env, jstream_bundle, streamRawMeta, AIRSTASHM_KEY_BITRATE, NULL );

                if (0 == strcmp(type, AIRSTASHM_VAL_TYPE__VIDEO)) {
                    fillMetaInternal(env, jstream_bundle, streamRawMeta, AIRSTASHM_KEY_WIDTH, NULL );
                    fillMetaInternal(env, jstream_bundle, streamRawMeta, AIRSTASHM_KEY_HEIGHT, NULL );
                    fillMetaInternal(env, jstream_bundle, streamRawMeta, AIRSTASHM_KEY_FPS_NUM, NULL );
                    fillMetaInternal(env, jstream_bundle, streamRawMeta, AIRSTASHM_KEY_FPS_DEN, NULL );
                    fillMetaInternal(env, jstream_bundle, streamRawMeta, AIRSTASHM_KEY_TBR_NUM, NULL );
                    fillMetaInternal(env, jstream_bundle, streamRawMeta, AIRSTASHM_KEY_TBR_DEN, NULL );
                    fillMetaInternal(env, jstream_bundle, streamRawMeta, AIRSTASHM_KEY_SAR_NUM, NULL );
                    fillMetaInternal(env, jstream_bundle, streamRawMeta, AIRSTASHM_KEY_SAR_DEN, NULL );
                } else if (0 == strcmp(type, AIRSTASHM_VAL_TYPE__AUDIO)) {
                    fillMetaInternal(env, jstream_bundle, streamRawMeta, AIRSTASHM_KEY_SAMPLE_RATE, NULL );
                    fillMetaInternal(env, jstream_bundle, streamRawMeta, AIRSTASHM_KEY_CHANNEL_LAYOUT, NULL );
                }
                AirStashC_ArrayList__add(env, jarray_list, jstream_bundle);
                if (AirStash_ExceptionCheck__throwAny(env)) {
                    goto LABEL_RETURN;
                }
            }

            SDL_JNI_DeleteLocalRefP(env, &jstream_bundle);
        }
    }

    AirStashC_Bundle__putParcelableArrayList__withCString__catchAll(env, jlocal_bundle, AIRSTASHM_KEY_STREAMS, jarray_list);
    jret_bundle = jlocal_bundle;
    jlocal_bundle = NULL;
LABEL_RETURN:
    if (is_locked && meta)
        airstashmeta_unlock(meta);

    SDL_JNI_DeleteLocalRefP(env, &jstream_bundle);
    SDL_JNI_DeleteLocalRefP(env, &jlocal_bundle);
    SDL_JNI_DeleteLocalRefP(env, &jarray_list);

    airstashmp_dec_ref_p(&mp);
    return jret_bundle;
}

static void
AirStashMediaPlayer_native_init(JNIEnv *env)
{
    MPTRACE("%s\n", __func__);
}

static void
AirStashMediaPlayer_native_setup(JNIEnv *env, jobject thiz, jobject weak_this)
{
    MPTRACE("%s\n", __func__);
    AirStashMediaPlayer *mp = airstashmp_android_create(message_loop);
    JNI_CHECK_GOTO(mp, env, "java/lang/OutOfMemoryError", "mpjni: native_setup: airstashmp_create() failed", LABEL_RETURN);

    jni_set_media_player(env, thiz, mp);
    airstashmp_set_weak_thiz(mp, (*env)->NewGlobalRef(env, weak_this));
    airstashmp_set_inject_opaque(mp, airstashmp_get_weak_thiz(mp));
    airstashmp_android_set_mediacodec_select_callback(mp, mediacodec_select_callback, (*env)->NewGlobalRef(env, weak_this));

LABEL_RETURN:
    airstashmp_dec_ref_p(&mp);
}

static void
AirStashMediaPlayer_native_finalize(JNIEnv *env, jobject thiz, jobject name, jobject value)
{
    MPTRACE("%s\n", __func__);
    AirStashMediaPlayer_release(env, thiz);
}

// NOTE: support to be called from read_thread
static int
inject_callback(void *opaque, int what, void *data, size_t data_size)
{
    JNIEnv     *env     = NULL;
    jobject     jbundle = NULL;
    int         ret     = -1;
    int         is_handled = 0;
    SDL_JNI_SetupThreadEnv(&env);

    jobject weak_thiz = (jobject) opaque;
    if (weak_thiz == NULL )
        goto fail;

    switch (what) {
    case AIRSTASHAVINJECT_CONCAT_RESOLVE_SEGMENT:
    case AIRSTASHAVINJECT_ON_TCP_OPEN:
    case AIRSTASHAVINJECT_ON_HTTP_OPEN:
    case AIRSTASHAVINJECT_ON_HTTP_RETRY:
    case AIRSTASHAVINJECT_ON_LIVE_RETRY: {
        AirStashAVInject_OnUrlOpenData *real_data = (AirStashAVInject_OnUrlOpenData *) data;
        real_data->is_handled = 0;

        jbundle = AirStashC_Bundle__Bundle__catchAll(env);
        if (!jbundle) {
            ALOGE("%s: ASDK_Bundle__init failed\n", __func__);
            goto fail;
        }

        AirStashC_Bundle__putString__withCString__catchAll(env, jbundle,  "url",           real_data->url);
        AirStashC_Bundle__putInt__withCString__catchAll(env, jbundle,     "segment_index", real_data->segment_index);
        AirStashC_Bundle__putInt__withCString__catchAll(env, jbundle,     "retry_counter", real_data->retry_counter);

        is_handled = AirStashC_AirStashMediaPlayer__onNativeInvoke__catchAll(env, weak_thiz, what, jbundle);
        if (AirStash_ExceptionCheck__catchAll(env))
            goto fail;

        AirStashC_Bundle__getString__withCString__asCBuffer(env, jbundle, "url", real_data->url, sizeof(real_data->url));
        if (AirStash_ExceptionCheck__catchAll(env))
            goto fail;

        real_data->is_handled = is_handled;
        ret = 0;
        break;
    }
    default: {
        goto fail;
    }
    }

fail:
    SDL_JNI_DeleteLocalRefP(env, &jbundle);
    return ret;
}

static bool mediacodec_select_callback(void *opaque, airstashmp_mediacodecinfo_context *mcc)
{
    JNIEnv *env = NULL;
    jobject weak_this = (jobject) opaque;
    const char *found_codec_name = NULL;

    if (JNI_OK != SDL_JNI_SetupThreadEnv(&env)) {
        ALOGE("%s: SetupThreadEnv failed\n", __func__);
        return -1;
    }

    found_codec_name = AirStashC_AirStashMediaPlayer__onSelectCodec__withCString__asCBuffer(env, weak_this, mcc->mime_type, mcc->profile, mcc->level, mcc->codec_name, sizeof(mcc->codec_name));
    if (AirStash_ExceptionCheck__catchAll(env) || !found_codec_name) {
        ALOGE("%s: onSelectCodec failed\n", __func__);
        goto fail;
    }

fail:
    return found_codec_name;
}

inline static void post_event(JNIEnv *env, jobject weak_this, int what, int arg1, int arg2)
{
    // MPTRACE("post_event(%p, %p, %d, %d, %d)", (void*)env, (void*) weak_this, what, arg1, arg2);
    AirStashC_AirStashMediaPlayer__postEventFromNative(env, weak_this, what, arg1, arg2, NULL);
    // MPTRACE("post_event()=void");
}

static void message_loop_n(JNIEnv *env, AirStashMediaPlayer *mp)
{
    jobject weak_thiz = (jobject) airstashmp_get_weak_thiz(mp);
    JNI_CHECK_GOTO(weak_thiz, env, NULL, "mpjni: message_loop_n: null weak_thiz", LABEL_RETURN);

    while (1) {
        AVMessage msg;

        int retval = airstashmp_get_msg(mp, &msg, 1);
        if (retval < 0)
            break;

        // block-get should never return 0
        assert(retval > 0);

        switch (msg.what) {
        case FFP_MSG_FLUSH:
            MPTRACE("FFP_MSG_FLUSH:\n");
            post_event(env, weak_thiz, MEDIA_NOP, 0, 0);
            break;
        case FFP_MSG_ERROR:
            MPTRACE("FFP_MSG_ERROR: %d\n", msg.arg1);
            post_event(env, weak_thiz, MEDIA_ERROR, MEDIA_ERROR_AIRSTASH_PLAYER, msg.arg1);
            break;
        case FFP_MSG_PREPARED:
            MPTRACE("FFP_MSG_PREPARED:\n");
            post_event(env, weak_thiz, MEDIA_PREPARED, 0, 0);
            break;
        case FFP_MSG_COMPLETED:
            MPTRACE("FFP_MSG_COMPLETED:\n");
            post_event(env, weak_thiz, MEDIA_PLAYBACK_COMPLETE, 0, 0);
            break;
        case FFP_MSG_VIDEO_SIZE_CHANGED:
            MPTRACE("FFP_MSG_VIDEO_SIZE_CHANGED: %d, %d\n", msg.arg1, msg.arg2);
            post_event(env, weak_thiz, MEDIA_SET_VIDEO_SIZE, msg.arg1, msg.arg2);
            break;
        case FFP_MSG_SAR_CHANGED:
            MPTRACE("FFP_MSG_SAR_CHANGED: %d, %d\n", msg.arg1, msg.arg2);
            post_event(env, weak_thiz, MEDIA_SET_VIDEO_SAR, msg.arg1, msg.arg2);
            break;
        case FFP_MSG_VIDEO_RENDERING_START:
            MPTRACE("FFP_MSG_VIDEO_RENDERING_START:\n");
            post_event(env, weak_thiz, MEDIA_INFO, MEDIA_INFO_VIDEO_RENDERING_START, 0);
            break;
        case FFP_MSG_AUDIO_RENDERING_START:
            MPTRACE("FFP_MSG_AUDIO_RENDERING_START:\n");
            post_event(env, weak_thiz, MEDIA_INFO, MEDIA_INFO_AUDIO_RENDERING_START, 0);
            break;
        case FFP_MSG_VIDEO_ROTATION_CHANGED:
            MPTRACE("FFP_MSG_VIDEO_ROTATION_CHANGED: %d\n", msg.arg1);
            post_event(env, weak_thiz, MEDIA_INFO, MEDIA_INFO_VIDEO_ROTATION_CHANGED, msg.arg1);
            break;
        case FFP_MSG_BUFFERING_START:
            MPTRACE("FFP_MSG_BUFFERING_START:\n");
            post_event(env, weak_thiz, MEDIA_INFO, MEDIA_INFO_BUFFERING_START, 0);
            break;
        case FFP_MSG_BUFFERING_END:
            MPTRACE("FFP_MSG_BUFFERING_END:\n");
            post_event(env, weak_thiz, MEDIA_INFO, MEDIA_INFO_BUFFERING_END, 0);
            break;
        case FFP_MSG_BUFFERING_UPDATE:
            // MPTRACE("FFP_MSG_BUFFERING_UPDATE: %d, %d", msg.arg1, msg.arg2);
            post_event(env, weak_thiz, MEDIA_BUFFERING_UPDATE, msg.arg1, msg.arg2);
            break;
        case FFP_MSG_BUFFERING_BYTES_UPDATE:
            break;
        case FFP_MSG_BUFFERING_TIME_UPDATE:
            break;
        case FFP_MSG_SEEK_COMPLETE:
            MPTRACE("FFP_MSG_SEEK_COMPLETE:\n");
            post_event(env, weak_thiz, MEDIA_SEEK_COMPLETE, 0, 0);
            break;
        case FFP_MSG_PLAYBACK_STATE_CHANGED:
            break;
        default:
            ALOGE("unknown FFP_MSG_xxx(%d)\n", msg.what);
            break;
        }
    }

LABEL_RETURN:
    ;
}

static int message_loop(void *arg)
{
    MPTRACE("%s\n", __func__);

    JNIEnv *env = NULL;
    (*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL );

    AirStashMediaPlayer *mp = (AirStashMediaPlayer*) arg;
    JNI_CHECK_GOTO(mp, env, NULL, "mpjni: native_message_loop: null mp", LABEL_RETURN);

    message_loop_n(env, mp);

LABEL_RETURN:
    airstashmp_dec_ref_p(&mp);
    (*g_jvm)->DetachCurrentThread(g_jvm);

    MPTRACE("message_loop exit");
    return 0;
}

// ----------------------------------------------------------------------------
void monstartup(const char *libname);
void moncleanup(void);

static void
AirStashMediaPlayer_native_profileBegin(JNIEnv *env, jclass clazz, jstring libName)
{
    MPTRACE("%s\n", __func__);

    const char *c_lib_name = NULL;
    static int s_monstartup = 0;

    if (!libName)
        return;

    if (s_monstartup) {
        ALOGW("monstartup already called\b");
        return;
    }

    c_lib_name = (*env)->GetStringUTFChars(env, libName, NULL );
    JNI_CHECK_GOTO(c_lib_name, env, "java/lang/OutOfMemoryError", "mpjni: monstartup: libName.string oom", LABEL_RETURN);

    s_monstartup = 1;
    monstartup(c_lib_name);
    ALOGD("monstartup: %s\n", c_lib_name);

LABEL_RETURN:
    if (c_lib_name)
        (*env)->ReleaseStringUTFChars(env, libName, c_lib_name);
}

static void
AirStashMediaPlayer_native_profileEnd(JNIEnv *env, jclass clazz)
{
    MPTRACE("%s\n", __func__);
    static int s_moncleanup = 0;

    if (s_moncleanup) {
        ALOGW("moncleanu already called\b");
        return;
    }

    s_moncleanup = 1;
    moncleanup();
    ALOGD("moncleanup\n");
}

static void
AirStashMediaPlayer_native_setLogLevel(JNIEnv *env, jclass clazz, jint level)
{
    MPTRACE("%s(%d)\n", __func__, level);
    airstashmp_global_set_log_level(level);
    ALOGD("moncleanup\n");
}




// ----------------------------------------------------------------------------

static JNINativeMethod g_methods[] = {
    {
        "_setDataSource",
        "(Ljava/lang/String;[Ljava/lang/String;[Ljava/lang/String;)V",
        (void *) AirStashMediaPlayer_setDataSourceAndHeaders
    },
    { "_setDataSourceFd",       "(I)V",     (void *) AirStashMediaPlayer_setDataSourceFd },
    { "_setDataSource",         "(Lcom/AirStash/player/player/misc/IMediaDataSource;)V", (void *)AirStashMediaPlayer_setDataSourceCallback },

    { "_setVideoSurface",       "(Landroid/view/Surface;)V", (void *) AirStashMediaPlayer_setVideoSurface },
    { "_prepareAsync",          "()V",      (void *) AirStashMediaPlayer_prepareAsync },
    { "_start",                 "()V",      (void *) AirStashMediaPlayer_start },
    { "_stop",                  "()V",      (void *) AirStashMediaPlayer_stop },
    { "seekTo",                 "(J)V",     (void *) AirStashMediaPlayer_seekTo },
    { "_pause",                 "()V",      (void *) AirStashMediaPlayer_pause },
    { "isPlaying",              "()Z",      (void *) AirStashMediaPlayer_isPlaying },
    { "getCurrentPosition",     "()J",      (void *) AirStashMediaPlayer_getCurrentPosition },
    { "getDuration",            "()J",      (void *) AirStashMediaPlayer_getDuration },
    { "_release",               "()V",      (void *) AirStashMediaPlayer_release },
    { "_reset",                 "()V",      (void *) AirStashMediaPlayer_reset },
    { "setVolume",              "(FF)V",    (void *) AirStashMediaPlayer_setVolume },
    { "getAudioSessionId",      "()I",      (void *) AirStashMediaPlayer_getAudioSessionId },
    { "native_init",            "()V",      (void *) AirStashMediaPlayer_native_init },
    { "native_setup",           "(Ljava/lang/Object;)V", (void *) AirStashMediaPlayer_native_setup },
    { "native_finalize",        "()V",      (void *) AirStashMediaPlayer_native_finalize },

    { "_setOption",             "(ILjava/lang/String;Ljava/lang/String;)V", (void *) AirStashMediaPlayer_setOption },
    { "_setOption",             "(ILjava/lang/String;J)V",                  (void *) AirStashMediaPlayer_setOptionLong },

    { "_getColorFormatName",    "(I)Ljava/lang/String;",    (void *) AirStashMediaPlayer_getColorFormatName },
    { "_getVideoCodecInfo",     "()Ljava/lang/String;",     (void *) AirStashMediaPlayer_getVideoCodecInfo },
    { "_getAudioCodecInfo",     "()Ljava/lang/String;",     (void *) AirStashMediaPlayer_getAudioCodecInfo },
    { "_getMediaMeta",          "()Landroid/os/Bundle;",    (void *) AirStashMediaPlayer_getMediaMeta },
    { "_setLoopCount",          "(I)V",                     (void *) AirStashMediaPlayer_setLoopCount },
    { "_getLoopCount",          "()I",                      (void *) AirStashMediaPlayer_getLoopCount },
    { "_getPropertyFloat",      "(IF)F",                    (void *) AirStashMediaPlayer_getPropertyFloat },
    { "_setPropertyFloat",      "(IF)V",                    (void *) AirStashMediaPlayer_setPropertyFloat },
    { "_getPropertyLong",       "(IJ)J",                    (void *) AirStashMediaPlayer_getPropertyLong },
    { "_setPropertyLong",       "(IJ)V",                    (void *) AirStashMediaPlayer_setPropertyLong },
    { "_setStreamSelected",     "(IZ)V",                    (void *) AirStashMediaPlayer_setStreamSelected },

    { "native_profileBegin",    "(Ljava/lang/String;)V",    (void *) AirStashMediaPlayer_native_profileBegin },
    { "native_profileEnd",      "()V",                      (void *) AirStashMediaPlayer_native_profileEnd },

    { "native_setLogLevel",     "(I)V",                     (void *) AirStashMediaPlayer_native_setLogLevel },
};

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    JNIEnv* env = NULL;

    g_jvm = vm;
    if ((*vm)->GetEnv(vm, (void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        return -1;
    }
    assert(env != NULL);

    pthread_mutex_init(&g_clazz.mutex, NULL );

    // FindClass returns LocalReference
    AIRSTASH_FIND_JAVA_CLASS(env, g_clazz.clazz, JNI_CLASS_AIRSTASHPLAYER);
    (*env)->RegisterNatives(env, g_clazz.clazz, g_methods, NELEM(g_methods) );

    airstashmp_global_init();
    airstashmp_global_set_inject_callback(inject_callback);

    FFmpegApi_global_init(env);

    return JNI_VERSION_1_4;
}

JNIEXPORT void JNI_OnUnload(JavaVM *jvm, void *reserved)
{
    airstashmp_global_uninit();

    pthread_mutex_destroy(&g_clazz.mutex);
}
