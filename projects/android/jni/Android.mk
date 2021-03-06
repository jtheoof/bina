# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

TOP_PATH := $(call my-dir)
LOCAL_PATH := $(call my-dir)

include $(call all-subdir-makefiles)

LOCAL_PATH := $(TOP_PATH)/../../../src
include $(CLEAR_VARS)

LOCAL_MODULE           := libbina
LOCAL_STATIC_LIBRARIES := libpng
LOCAL_CFLAGS           := \
    -std=c1x        \
    -Werror         \
    -DENABLE_PNG    \
    -DENABLE_S3TC   \
    -DHAVE_STDLIB_H \
    -DHAVE_STDIO_H  \
    -DHAVE_STRING_H \
    -DHAVE_CTYPE_H  \
    -DHAVE_MATH_H   \
    -DHAVE_TIME_H   \
    -DHAVE_PNG_H    \
    -DANDROID       \
    -DDEBUG

LOCAL_LDLIBS     := -landroid -llog -lGLESv2 -lz
LOCAL_C_INCLUDES := \
    $(LOCAL_PATH) \
    $(TOP_PATH)/lib/libpng
LOCAL_SRC_FILES  := \
    main/android.c \
    algebra.c      \
    camera.c       \
    game.c         \
    memory.c       \
    renderer.c     \
    scene.c        \
    shader.c       \
    sprite.c       \
    texture.c      \
    utils.c        \
    tc_png.c       \
    tc_s3tc.c

include $(BUILD_SHARED_LIBRARY)
