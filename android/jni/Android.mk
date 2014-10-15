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
LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := jEngine
LOCAL_CFLAGS    := -Werror

FILE_LIST_ENGINE := $(wildcard $(LOCAL_PATH)/../../engine/*.cpp) \
				   $(wildcard $(LOCAL_PATH)/../../engine/2d/*.cpp) \
				   $(wildcard $(LOCAL_PATH)/../../engine/action/*.cpp) \
				   $(wildcard $(LOCAL_PATH)/../../engine/core/*.cpp) \
				   $(wildcard $(LOCAL_PATH)/../../engine/component/*.cpp) \
				   $(wildcard $(LOCAL_PATH)/../../engine/event/*.cpp) \
				   $(wildcard $(LOCAL_PATH)/../../engine/math/*.cpp) \
				   $(wildcard $(LOCAL_PATH)/../../engine/platform/*.cpp) \
				   $(wildcard $(LOCAL_PATH)/../../engine/renderer/*.cpp) \
				   $(wildcard $(LOCAL_PATH)/../../engine/scene/*.cpp) \
				   $(wildcard $(LOCAL_PATH)/../../engine/texture/*.cpp)

FILE_LIST_GAME := $(wildcard $(LOCAL_PATH)/../../example/SwipeBlocks/*.cpp)
LOCAL_SRC_FILES := $(FILE_LIST_ENGINE) \
					$(FILE_LIST_GAME)\
					$(wildcard $(LOCAL_PATH)/*.cpp)
					
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../engine/ \
					$(LOCAL_PATH)/../../engine/2d/ \
					$(LOCAL_PATH)/../../engine/action/ \
					$(LOCAL_PATH)/../../engine/component/ \
					$(LOCAL_PATH)/../../engine/core/ \
					$(LOCAL_PATH)/../../engine/event/ \
					$(LOCAL_PATH)/../../engine/math/ \
					$(LOCAL_PATH)/../../engine/platform/ \
					$(LOCAL_PATH)/../../engine/renderer/ \
					$(LOCAL_PATH)/../../engine/scene/ \
					$(LOCAL_PATH)/../../engine/texture/ \
					$(LOCAL_PATH)/../../example/SwipeBlocks/

LOCAL_CFLAGS :=

LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv2 
LOCAL_STATIC_LIBRARIES := 
include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/ndk_helper)
$(call import-module,android/native_app_glue)

LOCAL_CPPFLAGS := -std=c++11 -pthread
