/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// OpenGL ES 2.0 code

#include "bina.h"

/**
 * Number of nanoseconds since last frame was drawn. */
uint64_t elap_time_g = 0;

float
main_get_time_elapsed()
{
    return (elap_time_g) / 1000000000.0f;
}

JNIEXPORT void JNICALL
Java_com_android_gl2jni_GL2JNILib_touch(JNIEnv * env, jobject obj,
                                        jfloat x, jfloat y)
{
    vec2_t coord;
    coord.x = 2.0f * x / viewport.width;
    coord.y = 2.0f - (2.0f * y / viewport.height);

    LOGD("Screen: %d,%d", x, y);
    LOGD("Viewport: %f,%f", coord.x, coord.y);

    bina_animate_brian_to(coord, 1.0f);
}

JNIEXPORT void JNICALL
Java_com_android_gl2jni_GL2JNILib_step(JNIEnv * env, jobject obj)
{
    static int64_t prev_time = 0;
    static int64_t curr_time = 0;

    struct timespec t;

    t.tv_sec = t.tv_nsec = 0;
    if (clock_gettime(CLOCK_MONOTONIC, &t)) {
        LOGE("clock_gettime returned an error");
    } else {
        prev_time = curr_time;
        curr_time = (int64_t) (t.tv_sec) * 1000000000LL + t.tv_nsec;
    }

    elap_time_g = curr_time - prev_time;

    renderer_render();
}

JNIEXPORT void JNICALL
Java_com_android_gl2jni_GL2JNILib_init(JNIEnv * env, jobject obj,
                                       jobject asset_manager,
                                       jint width, jint height)
{
    asset_manager_g = AAssetManager_fromJava(env, asset_manager);
    LOGI("Initializing bina with viewport: %dx%d", width, height);
    bina_init(width, height);
}
