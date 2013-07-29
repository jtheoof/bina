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
