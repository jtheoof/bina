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

LOCAL_PATH := $(TOP_PATH)
include $(CLEAR_VARS)

LOCAL_MODULE           := libgl2jni
LOCAL_CFLAGS           := -std=c99 -Werror -DHAVE_STDLIB_H -DHAVE_PNG_H
LOCAL_LDLIBS           := -llog -lGLESv2 -lz
LOCAL_STATIC_LIBRARIES := libpng
LOCAL_C_INCLUDES       := \
	$(LOCAL_PATH) \
	$(LOCAL_PATH)/lib/libpng
LOCAL_SRC_FILES        :=\
	../../utils.c \
	../../math.c \
	../../texture.c \
	../../sprite.c \
	../../shader.c \
	../../camera.c \
	../../renderer.c \
	../../bina.c \
	../../main_android.c

include $(BUILD_SHARED_LIBRARY)