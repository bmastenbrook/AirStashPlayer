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
 * AirStashMetadataRetriever is based upon FFmpegMediaMetadataRetriever:
 * Copyright 2015 William Seemann
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


#define LOG_TAG "AirStashMetadataRetriever"

#include <assert.h>
#include <android/log.h>
#include <mediametadataretriever.h>
#include "jni.h"

extern "C" {
#include "ffmpeg_mediametadataretriever.h"
}

using namespace std;

struct fields_t {
    jfieldID context;
    jfieldID context64;
};

static fields_t fields;
static const char* const kClassPathName = "com/AirStash/player/thumbnails/AirStashFFVideoThumbnailer";

static JavaVM *m_vm;

static jstring NewStringUTF(JNIEnv* env, const char * data) {
    jstring str = NULL;
    
    int size = strlen(data);
    
    jbyteArray array = NULL;
    array = env->NewByteArray(size);
    if (!array) {
        __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "convertString: OutOfMemoryError is thrown.");
    } else {
        jbyte* bytes = env->GetByteArrayElements(array, NULL);
        if (bytes != NULL) {
            memcpy(bytes, data, size);
            env->ReleaseByteArrayElements(array, bytes, 0);
            
            jclass string_Clazz = env->FindClass("java/lang/String");
            jmethodID string_initMethodID = env->GetMethodID(string_Clazz, "<init>", "([BLjava/lang/String;)V");
            jstring utf = env->NewStringUTF("UTF-8");
            str = (jstring) env->NewObject(string_Clazz, string_initMethodID, array, utf);
            
            env->DeleteLocalRef(utf);
        }
    }
    env->DeleteLocalRef(array);
    
    return str;
}

void jniThrowException(JNIEnv* env, const char* className,
                       const char* msg) {
    jclass exception = env->FindClass(className);
    env->ThrowNew(exception, msg);
}

static void process_media_retriever_call(JNIEnv *env, int opStatus, const char* exception, const char *message)
{
    if (opStatus == -2) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
    } else if (opStatus == -1) {
        if (strlen(message) > 230) {
            jniThrowException( env, exception, message);
        } else {
            char msg[256];
            sprintf(msg, "%s: status = 0x%X", message, opStatus);
            jniThrowException( env, exception, msg);
        }
    }
}

static MediaMetadataRetriever* getRetriever(JNIEnv* env, jobject thiz)
{
#ifdef ARCH_ABI_64
    int x = env->GetIntField(thiz, fields.context);
    int y = env->GetIntField(thiz, fields.context64);
    MediaMetadataRetriever* retriever = (MediaMetadataRetriever*)(((long unsigned int) x) << 32 | y);
    return retriever;
#else
    MediaMetadataRetriever* retriever = (MediaMetadataRetriever*) env->GetIntField(thiz, fields.context);
    return retriever;
#endif
}

static void setRetriever(JNIEnv* env, jobject thiz, int x, int y)
{
    env->SetIntField(thiz, fields.context, x);
    env->SetIntField(thiz, fields.context64, y);
}


static void
AirStashFFmpegMediaMetadataRetriever_setDataSourceAndHeaders(
                                                                    JNIEnv *env, jobject thiz, jstring path,
                                                                    jobjectArray keys, jobjectArray values) {
    
    __android_log_write(ANDROID_LOG_VERBOSE, LOG_TAG, "setDataSource");
    MediaMetadataRetriever* retriever = getRetriever(env, thiz);
    if (retriever == 0) {
        jniThrowException(env, "java/lang/IllegalStateException", "No retriever available");
        return;
    }
    
    if (!path) {
        jniThrowException(env, "java/lang/IllegalArgumentException", "Null pointer");
        return;
    }
    
    const char *tmp = env->GetStringUTFChars(path, NULL);
    if (!tmp) {
        return;
    }
    
    // Don't let somebody trick us in to reading some random block of memory
    if (strncmp("mem://", tmp, 6) == 0) {
        jniThrowException(env, "java/lang/IllegalArgumentException", "Invalid pathname");
        return;
    }
    
    // Workaround for FFmpeg ticket #998
    // "must convert mms://... streams to mmsh://... for FFmpeg to work"
    char *restrict_to = strstr(tmp, "mms://");
    if (restrict_to) {
        strncpy(restrict_to, "mmsh://", 6);
        puts(tmp);
    }
    
    char *headers = NULL;
    
    if (keys && values != NULL) {
        int keysCount = env->GetArrayLength(keys);
        int valuesCount = env->GetArrayLength(values);
        
        if (keysCount != valuesCount) {
            __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "keys and values arrays have different length");
            jniThrowException(env, "java/lang/IllegalArgumentException", NULL);
            return;
        }
        
        int i = 0;
        const char *rawString = NULL;
        char hdrs[2048];
        
        for (i = 0; i < keysCount; i++) {
            jstring key = (jstring) env->GetObjectArrayElement(keys, i);
            rawString = env->GetStringUTFChars(key, NULL);
            strcat(hdrs, rawString);
            strcat(hdrs, ": ");
            env->ReleaseStringUTFChars(key, rawString);
            
            jstring value = (jstring) env->GetObjectArrayElement(values, i);
            rawString = env->GetStringUTFChars(value, NULL);
            strcat(hdrs, rawString);
            strcat(hdrs, "\r\n");
            env->ReleaseStringUTFChars(value, rawString);
        }
        
        headers = &hdrs[0];
    }
    
    process_media_retriever_call(
                                 env,
                                 retriever->setDataSource(tmp, headers),
                                 "java/lang/IllegalArgumentException",
                                 "setDataSource failed");
    
    env->ReleaseStringUTFChars(path, tmp);
    tmp = NULL;
}

static void AirStashFFmpegMediaMetadataRetriever_setDataSource(
                                                                      JNIEnv *env, jobject thiz, jstring path) {
    AirStashFFmpegMediaMetadataRetriever_setDataSourceAndHeaders(
                                                                        env, thiz, path, NULL, NULL);
}

static int jniGetFDFromFileDescriptor(JNIEnv * env, jobject fileDescriptor) {
    jint fd = -1;
    jclass fdClass = env->FindClass("java/io/FileDescriptor");
    
    if (fdClass != NULL) {
        jfieldID fdClassDescriptorFieldID = env->GetFieldID(fdClass, "descriptor", "I");
        if (fdClassDescriptorFieldID != NULL && fileDescriptor != NULL) {
            fd = env->GetIntField(fileDescriptor, fdClassDescriptorFieldID);
        }
    }
    
    return fd;
}

static void AirStashFFmpegMediaMetadataRetriever_setDataSourceFD(JNIEnv *env, jobject thiz, jobject fileDescriptor, jlong offset, jlong length)
{
    __android_log_write(ANDROID_LOG_VERBOSE, LOG_TAG, "setDataSource");
    MediaMetadataRetriever* retriever = getRetriever(env, thiz);
    if (retriever == 0) {
        jniThrowException(env, "java/lang/IllegalStateException", "No retriever available");
        return;
    }
    if (!fileDescriptor) {
        jniThrowException(env, "java/lang/IllegalArgumentException", NULL);
        return;
    }
    int fd = jniGetFDFromFileDescriptor(env, fileDescriptor);
    if (offset < 0 || length < 0 || fd < 0) {
        if (offset < 0) {
            __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "negative offset (%lld)", offset);
        }
        if (length < 0) {
            __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "negative length (%lld)", length);
        }
        if (fd < 0) {
            __android_log_write(ANDROID_LOG_ERROR, LOG_TAG, "invalid file descriptor");
        }
        jniThrowException(env, "java/lang/IllegalArgumentException", NULL);
        return;
    }
    process_media_retriever_call(env, retriever->setDataSource(fd, offset, length), "java/lang/RuntimeException", "setDataSource failed");
}

static jbyteArray AirStashFFmpegMediaMetadataRetriever_getFrameAtTime(JNIEnv *env, jobject thiz, jlong timeUs, jint option)
{
    __android_log_write(ANDROID_LOG_INFO, LOG_TAG, "getFrameAtTime");
    MediaMetadataRetriever* retriever = getRetriever(env, thiz);
    if (retriever == 0) {
        jniThrowException(env, "java/lang/IllegalStateException", "No retriever available");
        return NULL;
    }
    
    AVPacket packet;
    av_init_packet(&packet);
    jbyteArray array = NULL;
    
    if (retriever->getFrameAtTime(timeUs, option, &packet) == 0) {
        int size = packet.size;
        uint8_t* data = packet.data;
        array = env->NewByteArray(size);
        if (!array) {
            __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "getFrameAtTime: OutOfMemoryError is thrown.");
        } else {
            __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "getFrameAtTime: Got frame.");
            __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Frame Size: %d", size);
            jbyte* bytes = env->GetByteArrayElements(array, NULL);
            if (bytes != NULL) {
                memcpy(bytes, data, size);
                env->ReleaseByteArrayElements(array, bytes, 0);
            }
        }
    }
    
    av_free_packet(&packet);
    
    return array;
}

static jbyteArray AirStashFFmpegMediaMetadataRetriever_getScaledFrameAtTime(JNIEnv *env, jobject thiz, jlong timeUs, jint option, jint width, jint height)
{
    __android_log_write(ANDROID_LOG_INFO, LOG_TAG, "getScaledFrameAtTime");
    MediaMetadataRetriever* retriever = getRetriever(env, thiz);
    if (retriever == 0) {
        jniThrowException(env, "java/lang/IllegalStateException", "No retriever available");
        return NULL;
    }
    
    AVPacket packet;
    av_init_packet(&packet);
    jbyteArray array = NULL;
    
    if (retriever->getScaledFrameAtTime(timeUs, option, &packet, width, height) == 0) {
        int size = packet.size;
        uint8_t* data = packet.data;
        array = env->NewByteArray(size);
        if (!array) {
            __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "getFrameAtTime: OutOfMemoryError is thrown.");
        } else {
            __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "getFrameAtTime: Got frame.");
            jbyte* bytes = env->GetByteArrayElements(array, NULL);
            if (bytes != NULL) {
                memcpy(bytes, data, size);
                env->ReleaseByteArrayElements(array, bytes, 0);
            }
        }
    }
    
    av_free_packet(&packet);
    
    return array;
}

static jbyteArray AirStashFFmpegMediaMetadataRetriever_getEmbeddedPicture(JNIEnv *env, jobject thiz)
{
    __android_log_write(ANDROID_LOG_INFO, LOG_TAG, "getEmbeddedPicture");
    MediaMetadataRetriever* retriever = getRetriever(env, thiz);
    if (retriever == 0) {
        jniThrowException(env, "java/lang/IllegalStateException", "No retriever available");
        return NULL;
    }
    
    AVPacket packet;
    av_init_packet(&packet);
    jbyteArray array = NULL;
    
    if (retriever->extractAlbumArt(&packet) == 0) {
        int size = packet.size;
        uint8_t* data = packet.data;
        array = env->NewByteArray(size);
        if (!array) {
            __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "getEmbeddedPicture: OutOfMemoryError is thrown.");
        } else {
            __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "getEmbeddedPicture: Found album art.");
            jbyte* bytes = env->GetByteArrayElements(array, NULL);
            if (bytes != NULL) {
                memcpy(bytes, data, size);
                env->ReleaseByteArrayElements(array, bytes, 0);
            }
        }
    }
    
    av_free_packet(&packet);
    
    return array;
}

static jobject AirStashFFmpegMediaMetadataRetriever_extractMetadata(JNIEnv *env, jobject thiz, jstring jkey)
{
    __android_log_write(ANDROID_LOG_INFO, LOG_TAG, "extractMetadata");
    MediaMetadataRetriever* retriever = getRetriever(env, thiz);
    if (retriever == 0) {
        jniThrowException(env, "java/lang/IllegalStateException", "No retriever available");
        return NULL;
    }
    
    if (!jkey) {
        jniThrowException(env, "java/lang/IllegalArgumentException", "Null pointer");
        return NULL;
    }
    
    const char *key = env->GetStringUTFChars(jkey, NULL);
    if (!key) {  // OutOfMemoryError exception already thrown
        return NULL;
    }
    
    const char* value = retriever->extractMetadata(key);
    if (!value) {
        __android_log_write(ANDROID_LOG_INFO, LOG_TAG, "extractMetadata: Metadata is not found");
        return NULL;
    }
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "extractMetadata: value (%s) for keyCode(%s)", value, key);
    env->ReleaseStringUTFChars(jkey, key);
    return NewStringUTF(env, value);
}

static jobject AirStashFFmpegMediaMetadataRetriever_extractMetadataFromChapter(JNIEnv *env, jobject thiz, jstring jkey, jint chapter)
{
    __android_log_write(ANDROID_LOG_INFO, LOG_TAG, "extractMetadataFromChapter");
    MediaMetadataRetriever* retriever = getRetriever(env, thiz);
    if (retriever == 0) {
        jniThrowException(env, "java/lang/IllegalStateException", "No retriever available");
        return NULL;
    }
    
    if (!jkey) {
        jniThrowException(env, "java/lang/IllegalArgumentException", "Null pointer");
        return NULL;
    }
    
    const char *key = env->GetStringUTFChars(jkey, NULL);
    if (!key) {
        return NULL;
    }
    
    if (chapter <= 0) {
        return NULL;
    }
    
    const char* value = retriever->extractMetadataFromChapter(key, chapter);
    if (!value) {
        __android_log_write(ANDROID_LOG_INFO, LOG_TAG, "extractMetadata: Metadata is not found");
        return NULL;
    }
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "extractMetadata: value (%s) for keyCode(%s)", value, key);
    env->ReleaseStringUTFChars(jkey, key);
    return env->NewStringUTF(value);
}

static jobject
AirStashFFmpegMediaMetadataRetriever_getMetadata(JNIEnv *env, jobject thiz, jboolean update_only,
                                                        jboolean apply_filter, jobject reply)
{
    __android_log_write(ANDROID_LOG_INFO, LOG_TAG, "getMetadata");
    MediaMetadataRetriever* retriever = getRetriever(env, thiz);
    if (retriever == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return JNI_FALSE;
    }
    
    // On return metadata is positioned at the beginning of the
    // metadata. Note however that the parcel actually starts with the
    // return code so you should not rewind the parcel using
    // setDataPosition(0).
    AVDictionary *metadata = NULL;
    
    if (retriever->getMetadata(update_only, apply_filter, &metadata) == 0) {
        jclass hashMap_Clazz = env->FindClass("java/util/HashMap");
        jmethodID gHashMap_initMethodID = env->GetMethodID(hashMap_Clazz, "<init>", "()V");
        jobject map = env->NewObject(hashMap_Clazz, gHashMap_initMethodID);
        jmethodID gHashMap_putMethodID = env->GetMethodID(hashMap_Clazz, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
        
        int i = 0;
        
        for (i = 0; i < metadata->count; i++) {
            jstring jKey = NewStringUTF(env, metadata->elems[i].key);
            jstring jValue = NewStringUTF(env, metadata->elems[i].value);
            (jobject) env->CallObjectMethod(map, gHashMap_putMethodID, jKey, jValue);
            env->DeleteLocalRef(jKey);
            env->DeleteLocalRef(jValue);
        }
        
        if (metadata) {
            av_dict_free(&metadata);
            
        }
        
        return map;
    } else {
        return reply;
    }
}

static void AirStashFFmpegMediaMetadataRetriever_release(JNIEnv *env, jobject thiz)
{
    __android_log_write(ANDROID_LOG_INFO, LOG_TAG, "release");
    MediaMetadataRetriever* retriever = getRetriever(env, thiz);
    delete retriever;
    setRetriever(env, thiz, 0, 0);
}

static void AirStashFFmpegMediaMetadataRetriever_native_finalize(JNIEnv *env, jobject thiz)
{
    __android_log_write(ANDROID_LOG_INFO, LOG_TAG, "native_finalize");
    AirStashFFmpegMediaMetadataRetriever_release(env, thiz);
}

static void AirStashFFmpegMediaMetadataRetriever_native_init(JNIEnv *env, jobject thiz)
{
    __android_log_write(ANDROID_LOG_INFO, LOG_TAG, "native_init");
    jclass clazz = env->FindClass(kClassPathName);
    if (clazz == NULL) {
        return;
    }
    
    fields.context = env->GetFieldID(clazz, "mNativeContext", "I");
    if (fields.context == NULL) {
        return;
    }
    
    fields.context64 = env->GetFieldID(clazz, "mNativeContext64", "I");
    if(fields.context64 == NULL) {
        return;
    }

    av_register_all();
    avcodec_register_all();
    avformat_network_init();
}

static void AirStashFFmpegMediaMetadataRetriever_native_setup(JNIEnv *env, jobject thiz)
{
    __android_log_write(ANDROID_LOG_INFO, LOG_TAG, "native_setup");
    MediaMetadataRetriever* retriever = new MediaMetadataRetriever();
    if (retriever == 0) {
        jniThrowException(env, "java/lang/RuntimeException", "Out of memory");
        return;
    }
#ifdef ARCH_ABI_64
        int x = (int)(((long unsigned int)(retriever) & 0xFFFFFFFF00000000) >> 32);
        int y = (int)((long unsigned int)(retriever) & 0xFFFFFFFF);
        setRetriever(env, thiz, x, y);
#else
        setRetriever(env, thiz, (int)retriever, 0);
#endif
}

// JNI mapping between Java methods and native methods
static JNINativeMethod nativeMethods[] = {
    {"setDataSource", "(Ljava/lang/String;)V", (void *)AirStashFFmpegMediaMetadataRetriever_setDataSource},
    
    {
        "_setDataSource",
        "(Ljava/lang/String;[Ljava/lang/String;[Ljava/lang/String;)V",
        (void *)AirStashFFmpegMediaMetadataRetriever_setDataSourceAndHeaders
    },
    
    {"setDataSource", "(Ljava/io/FileDescriptor;JJ)V", (void *)AirStashFFmpegMediaMetadataRetriever_setDataSourceFD},
    {"_getFrameAtTime", "(JI)[B", (void *)AirStashFFmpegMediaMetadataRetriever_getFrameAtTime},
    {"_getScaledFrameAtTime", "(JIII)[B", (void *)AirStashFFmpegMediaMetadataRetriever_getScaledFrameAtTime},
    {"extractMetadata", "(Ljava/lang/String;)Ljava/lang/String;", (void *)AirStashFFmpegMediaMetadataRetriever_extractMetadata},
    {"extractMetadataFromChapter", "(Ljava/lang/String;I)Ljava/lang/String;", (void *)AirStashFFmpegMediaMetadataRetriever_extractMetadataFromChapter},
    {"native_getMetadata", "(ZZLjava/util/HashMap;)Ljava/util/HashMap;", (void *)AirStashFFmpegMediaMetadataRetriever_getMetadata},
    {"getEmbeddedPicture", "()[B", (void *)AirStashFFmpegMediaMetadataRetriever_getEmbeddedPicture},
    {"release", "()V", (void *)AirStashFFmpegMediaMetadataRetriever_release},
    {"native_finalize", "()V", (void *)AirStashFFmpegMediaMetadataRetriever_native_finalize},
    {"native_setup", "()V", (void *)AirStashFFmpegMediaMetadataRetriever_native_setup},
    {"native_init", "()V", (void *)AirStashFFmpegMediaMetadataRetriever_native_init},
};

int register_AirStashFFmpegMediaMetadataRetriever(JNIEnv *env)
{
    int numMethods = (sizeof(nativeMethods) / sizeof( (nativeMethods)[0]));
    jclass clazz = env->FindClass(kClassPathName);
    jint ret = env->RegisterNatives(clazz, nativeMethods, numMethods);
    env->DeleteLocalRef(clazz);
    return ret;
}

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    m_vm = vm;
    JNIEnv* env = NULL;
    jint result = -1;
    
    if (vm->GetEnv((void**) &env, JNI_VERSION_1_6) != JNI_OK) {
        __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "ERROR: GetEnv failed\n");
        goto bail;
    }
    assert(env != NULL);
    
    if (register_AirStashFFmpegMediaMetadataRetriever(env) < 0) {
        __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "ERROR: AirStashMetadataRetriever native registration failed\n");
        goto bail;
    }
    result = JNI_VERSION_1_6;
    
bail:
    return result;
}