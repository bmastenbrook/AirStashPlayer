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

#include "Bundle.h"

typedef struct AirStashC_Bundle {
    jclass id;

    jmethodID constructor_Bundle;
    jmethodID method_getInt;
    jmethodID method_putInt;
    jmethodID method_getString;
    jmethodID method_putString;
    jmethodID method_putParcelableArrayList;
} AirStashC_Bundle;
static AirStashC_Bundle class_AirStashC_Bundle;

jobject AirStashC_Bundle__Bundle(JNIEnv *env)
{
    return (*env)->NewObject(env, class_AirStashC_Bundle.id, class_AirStashC_Bundle.constructor_Bundle);
}

jobject AirStashC_Bundle__Bundle__catchAll(JNIEnv *env)
{
    jobject ret_object = AirStashC_Bundle__Bundle(env);
    if (AirStash_ExceptionCheck__catchAll(env) || !ret_object) {
        return NULL;
    }

    return ret_object;
}

jobject AirStashC_Bundle__Bundle__asGlobalRef__catchAll(JNIEnv *env)
{
    jobject ret_object   = NULL;
    jobject local_object = AirStashC_Bundle__Bundle__catchAll(env);
    if (AirStash_ExceptionCheck__catchAll(env) || !local_object) {
        ret_object = NULL;
        goto fail;
    }

    ret_object = AirStash_NewGlobalRef__catchAll(env, local_object);
    if (!ret_object) {
        ret_object = NULL;
        goto fail;
    }

fail:
    AirStash_DeleteLocalRef__p(env, &local_object);
    return ret_object;
}

jint AirStashC_Bundle__getInt(JNIEnv *env, jobject thiz, jstring key, jint defaultValue)
{
    return (*env)->CallIntMethod(env, thiz, class_AirStashC_Bundle.method_getInt, key, defaultValue);
}

jint AirStashC_Bundle__getInt__catchAll(JNIEnv *env, jobject thiz, jstring key, jint defaultValue)
{
    jint ret_value = AirStashC_Bundle__getInt(env, thiz, key, defaultValue);
    if (AirStash_ExceptionCheck__catchAll(env)) {
        return 0;
    }

    return ret_value;
}

jint AirStashC_Bundle__getInt__withCString(JNIEnv *env, jobject thiz, const char *key_cstr__, jint defaultValue)
{
    jint ret_value = 0;
    jstring key = NULL;

    key = (*env)->NewStringUTF(env, key_cstr__);
    if (AirStash_ExceptionCheck__throwAny(env) || !key)
        goto fail;

    ret_value = AirStashC_Bundle__getInt(env, thiz, key, defaultValue);
    if (AirStash_ExceptionCheck__throwAny(env)) {
        ret_value = 0;
        goto fail;
    }

fail:
    AirStash_DeleteLocalRef__p(env, &key);
    return ret_value;
}

jint AirStashC_Bundle__getInt__withCString__catchAll(JNIEnv *env, jobject thiz, const char *key_cstr__, jint defaultValue)
{
    jint ret_value = 0;
    jstring key = NULL;

    key = (*env)->NewStringUTF(env, key_cstr__);
    if (AirStash_ExceptionCheck__catchAll(env) || !key)
        goto fail;

    ret_value = AirStashC_Bundle__getInt__catchAll(env, thiz, key, defaultValue);
    if (AirStash_ExceptionCheck__catchAll(env)) {
        ret_value = 0;
        goto fail;
    }

fail:
    AirStash_DeleteLocalRef__p(env, &key);
    return ret_value;
}

void AirStashC_Bundle__putInt(JNIEnv *env, jobject thiz, jstring key, jint value)
{
    (*env)->CallVoidMethod(env, thiz, class_AirStashC_Bundle.method_putInt, key, value);
}

void AirStashC_Bundle__putInt__catchAll(JNIEnv *env, jobject thiz, jstring key, jint value)
{
    AirStashC_Bundle__putInt(env, thiz, key, value);
    AirStash_ExceptionCheck__catchAll(env);
}

void AirStashC_Bundle__putInt__withCString(JNIEnv *env, jobject thiz, const char *key_cstr__, jint value)
{
    jstring key = NULL;

    key = (*env)->NewStringUTF(env, key_cstr__);
    if (AirStash_ExceptionCheck__throwAny(env) || !key)
        goto fail;

    AirStashC_Bundle__putInt(env, thiz, key, value);

fail:
    AirStash_DeleteLocalRef__p(env, &key);
}

void AirStashC_Bundle__putInt__withCString__catchAll(JNIEnv *env, jobject thiz, const char *key_cstr__, jint value)
{
    jstring key = NULL;

    key = (*env)->NewStringUTF(env, key_cstr__);
    if (AirStash_ExceptionCheck__catchAll(env) || !key)
        goto fail;

    AirStashC_Bundle__putInt__catchAll(env, thiz, key, value);

fail:
    AirStash_DeleteLocalRef__p(env, &key);
}

jstring AirStashC_Bundle__getString(JNIEnv *env, jobject thiz, jstring key)
{
    return (*env)->CallObjectMethod(env, thiz, class_AirStashC_Bundle.method_getString, key);
}

jstring AirStashC_Bundle__getString__catchAll(JNIEnv *env, jobject thiz, jstring key)
{
    jstring ret_object = AirStashC_Bundle__getString(env, thiz, key);
    if (AirStash_ExceptionCheck__catchAll(env) || !ret_object) {
        return NULL;
    }

    return ret_object;
}

jstring AirStashC_Bundle__getString__asGlobalRef__catchAll(JNIEnv *env, jobject thiz, jstring key)
{
    jstring ret_object   = NULL;
    jstring local_object = AirStashC_Bundle__getString__catchAll(env, thiz, key);
    if (AirStash_ExceptionCheck__catchAll(env) || !local_object) {
        ret_object = NULL;
        goto fail;
    }

    ret_object = AirStash_NewGlobalRef__catchAll(env, local_object);
    if (!ret_object) {
        ret_object = NULL;
        goto fail;
    }

fail:
    AirStash_DeleteLocalRef__p(env, &local_object);
    return ret_object;
}

const char *AirStashC_Bundle__getString__asCBuffer(JNIEnv *env, jobject thiz, jstring key, char *out_buf, int out_len)
{
    const char *ret_value = NULL;
    const char *c_str     = NULL;
    jstring local_string = AirStashC_Bundle__getString(env, thiz, key);
    if (AirStash_ExceptionCheck__throwAny(env) || !local_string) {
        goto fail;
    }

    c_str = (*env)->GetStringUTFChars(env, local_string, NULL );
    if (AirStash_ExceptionCheck__throwAny(env) || !c_str) {
        goto fail;
    }

    strlcpy(out_buf, c_str, out_len);
    ret_value = out_buf;

fail:
    AirStash_ReleaseStringUTFChars__p(env, local_string, &c_str);
    AirStash_DeleteLocalRef__p(env, &local_string);
    return ret_value;
}

const char *AirStashC_Bundle__getString__asCBuffer__catchAll(JNIEnv *env, jobject thiz, jstring key, char *out_buf, int out_len)
{
    const char *ret_value = NULL;
    const char *c_str     = NULL;
    jstring local_string = AirStashC_Bundle__getString__catchAll(env, thiz, key);
    if (AirStash_ExceptionCheck__catchAll(env) || !local_string) {
        goto fail;
    }

    c_str = (*env)->GetStringUTFChars(env, local_string, NULL );
    if (AirStash_ExceptionCheck__catchAll(env) || !c_str) {
        goto fail;
    }

    strlcpy(out_buf, c_str, out_len);
    ret_value = out_buf;

fail:
    AirStash_ReleaseStringUTFChars__p(env, local_string, &c_str);
    AirStash_DeleteLocalRef__p(env, &local_string);
    return ret_value;
}

jstring AirStashC_Bundle__getString__withCString(JNIEnv *env, jobject thiz, const char *key_cstr__)
{
    jstring ret_object = NULL;
    jstring key = NULL;

    key = (*env)->NewStringUTF(env, key_cstr__);
    if (AirStash_ExceptionCheck__throwAny(env) || !key)
        goto fail;

    ret_object = AirStashC_Bundle__getString(env, thiz, key);
    if (AirStash_ExceptionCheck__throwAny(env) || !ret_object) {
        ret_object = NULL;
        goto fail;
    }

fail:
    AirStash_DeleteLocalRef__p(env, &key);
    return ret_object;
}

jstring AirStashC_Bundle__getString__withCString__catchAll(JNIEnv *env, jobject thiz, const char *key_cstr__)
{
    jstring ret_object = NULL;
    jstring key = NULL;

    key = (*env)->NewStringUTF(env, key_cstr__);
    if (AirStash_ExceptionCheck__catchAll(env) || !key)
        goto fail;

    ret_object = AirStashC_Bundle__getString__catchAll(env, thiz, key);
    if (AirStash_ExceptionCheck__catchAll(env) || !ret_object) {
        ret_object = NULL;
        goto fail;
    }

fail:
    AirStash_DeleteLocalRef__p(env, &key);
    return ret_object;
}

jstring AirStashC_Bundle__getString__withCString__asGlobalRef__catchAll(JNIEnv *env, jobject thiz, const char *key_cstr__)
{
    jstring ret_object   = NULL;
    jstring local_object = AirStashC_Bundle__getString__withCString__catchAll(env, thiz, key_cstr__);
    if (AirStash_ExceptionCheck__catchAll(env) || !local_object) {
        ret_object = NULL;
        goto fail;
    }

    ret_object = AirStash_NewGlobalRef__catchAll(env, local_object);
    if (!ret_object) {
        ret_object = NULL;
        goto fail;
    }

fail:
    AirStash_DeleteLocalRef__p(env, &local_object);
    return ret_object;
}

const char *AirStashC_Bundle__getString__withCString__asCBuffer(JNIEnv *env, jobject thiz, const char *key_cstr__, char *out_buf, int out_len)
{
    const char *ret_value = NULL;
    const char *c_str     = NULL;
    jstring local_string = AirStashC_Bundle__getString__withCString(env, thiz, key_cstr__);
    if (AirStash_ExceptionCheck__throwAny(env) || !local_string) {
        goto fail;
    }

    c_str = (*env)->GetStringUTFChars(env, local_string, NULL );
    if (AirStash_ExceptionCheck__throwAny(env) || !c_str) {
        goto fail;
    }

    strlcpy(out_buf, c_str, out_len);
    ret_value = out_buf;

fail:
    AirStash_ReleaseStringUTFChars__p(env, local_string, &c_str);
    AirStash_DeleteLocalRef__p(env, &local_string);
    return ret_value;
}

const char *AirStashC_Bundle__getString__withCString__asCBuffer__catchAll(JNIEnv *env, jobject thiz, const char *key_cstr__, char *out_buf, int out_len)
{
    const char *ret_value = NULL;
    const char *c_str     = NULL;
    jstring local_string = AirStashC_Bundle__getString__withCString__catchAll(env, thiz, key_cstr__);
    if (AirStash_ExceptionCheck__catchAll(env) || !local_string) {
        goto fail;
    }

    c_str = (*env)->GetStringUTFChars(env, local_string, NULL );
    if (AirStash_ExceptionCheck__catchAll(env) || !c_str) {
        goto fail;
    }

    strlcpy(out_buf, c_str, out_len);
    ret_value = out_buf;

fail:
    AirStash_ReleaseStringUTFChars__p(env, local_string, &c_str);
    AirStash_DeleteLocalRef__p(env, &local_string);
    return ret_value;
}

void AirStashC_Bundle__putString(JNIEnv *env, jobject thiz, jstring key, jstring value)
{
    (*env)->CallVoidMethod(env, thiz, class_AirStashC_Bundle.method_putString, key, value);
}

void AirStashC_Bundle__putString__catchAll(JNIEnv *env, jobject thiz, jstring key, jstring value)
{
    AirStashC_Bundle__putString(env, thiz, key, value);
    AirStash_ExceptionCheck__catchAll(env);
}

void AirStashC_Bundle__putString__withCString(JNIEnv *env, jobject thiz, const char *key_cstr__, const char *value_cstr__)
{
    jstring key = NULL;
    jstring value = NULL;

    key = (*env)->NewStringUTF(env, key_cstr__);
    if (AirStash_ExceptionCheck__throwAny(env) || !key)
        goto fail;
    value = (*env)->NewStringUTF(env, value_cstr__);
    if (AirStash_ExceptionCheck__throwAny(env) || !value)
        goto fail;

    AirStashC_Bundle__putString(env, thiz, key, value);

fail:
    AirStash_DeleteLocalRef__p(env, &key);
    AirStash_DeleteLocalRef__p(env, &value);
}

void AirStashC_Bundle__putString__withCString__catchAll(JNIEnv *env, jobject thiz, const char *key_cstr__, const char *value_cstr__)
{
    jstring key = NULL;
    jstring value = NULL;

    key = (*env)->NewStringUTF(env, key_cstr__);
    if (AirStash_ExceptionCheck__catchAll(env) || !key)
        goto fail;
    value = (*env)->NewStringUTF(env, value_cstr__);
    if (AirStash_ExceptionCheck__catchAll(env) || !value)
        goto fail;

    AirStashC_Bundle__putString__catchAll(env, thiz, key, value);

fail:
    AirStash_DeleteLocalRef__p(env, &key);
    AirStash_DeleteLocalRef__p(env, &value);
}

void AirStashC_Bundle__putParcelableArrayList(JNIEnv *env, jobject thiz, jstring key, jobject value)
{
    (*env)->CallVoidMethod(env, thiz, class_AirStashC_Bundle.method_putParcelableArrayList, key, value);
}

void AirStashC_Bundle__putParcelableArrayList__catchAll(JNIEnv *env, jobject thiz, jstring key, jobject value)
{
    AirStashC_Bundle__putParcelableArrayList(env, thiz, key, value);
    AirStash_ExceptionCheck__catchAll(env);
}

void AirStashC_Bundle__putParcelableArrayList__withCString(JNIEnv *env, jobject thiz, const char *key_cstr__, jobject value)
{
    jstring key = NULL;

    key = (*env)->NewStringUTF(env, key_cstr__);
    if (AirStash_ExceptionCheck__throwAny(env) || !key)
        goto fail;

    AirStashC_Bundle__putParcelableArrayList(env, thiz, key, value);

fail:
    AirStash_DeleteLocalRef__p(env, &key);
}

void AirStashC_Bundle__putParcelableArrayList__withCString__catchAll(JNIEnv *env, jobject thiz, const char *key_cstr__, jobject value)
{
    jstring key = NULL;

    key = (*env)->NewStringUTF(env, key_cstr__);
    if (AirStash_ExceptionCheck__catchAll(env) || !key)
        goto fail;

    AirStashC_Bundle__putParcelableArrayList__catchAll(env, thiz, key, value);

fail:
    AirStash_DeleteLocalRef__p(env, &key);
}

int AirStash_loadClass__AirStashC_Bundle(JNIEnv *env)
{
    int         ret                   = -1;
    const char *AIRSTASH_UNUSED(name)      = NULL;
    const char *AIRSTASH_UNUSED(sign)      = NULL;
    jclass      AIRSTASH_UNUSED(class_id)  = NULL;
    int         AIRSTASH_UNUSED(api_level) = 0;

    sign = "android/os/Bundle";
    class_AirStashC_Bundle.id = AirStash_FindClass__asGlobalRef__catchAll(env, sign);
    if (class_AirStashC_Bundle.id == NULL)
        goto fail;

    class_id = class_AirStashC_Bundle.id;
    name     = "<init>";
    sign     = "()V";
    class_AirStashC_Bundle.constructor_Bundle = AirStash_GetMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_Bundle.constructor_Bundle == NULL)
        goto fail;

    class_id = class_AirStashC_Bundle.id;
    name     = "getInt";
    sign     = "(Ljava/lang/String;I)I";
    class_AirStashC_Bundle.method_getInt = AirStash_GetMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_Bundle.method_getInt == NULL)
        goto fail;

    class_id = class_AirStashC_Bundle.id;
    name     = "putInt";
    sign     = "(Ljava/lang/String;I)V";
    class_AirStashC_Bundle.method_putInt = AirStash_GetMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_Bundle.method_putInt == NULL)
        goto fail;

    class_id = class_AirStashC_Bundle.id;
    name     = "getString";
    sign     = "(Ljava/lang/String;)Ljava/lang/String;";
    class_AirStashC_Bundle.method_getString = AirStash_GetMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_Bundle.method_getString == NULL)
        goto fail;

    class_id = class_AirStashC_Bundle.id;
    name     = "putString";
    sign     = "(Ljava/lang/String;Ljava/lang/String;)V";
    class_AirStashC_Bundle.method_putString = AirStash_GetMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_Bundle.method_putString == NULL)
        goto fail;

    class_id = class_AirStashC_Bundle.id;
    name     = "putParcelableArrayList";
    sign     = "(Ljava/lang/String;Ljava/util/ArrayList;)V";
    class_AirStashC_Bundle.method_putParcelableArrayList = AirStash_GetMethodID__catchAll(env, class_id, name, sign);
    if (class_AirStashC_Bundle.method_putParcelableArrayList == NULL)
        goto fail;

    ALOGD("AirStashLoader: OK: '%s' loaded\n", "android.os.Bundle");
    ret = 0;
fail:
    return ret;
}
