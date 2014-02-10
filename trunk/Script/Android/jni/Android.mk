# Amjulib for Android 
# Jason Colman 2014


LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := amju

LOCAL_CFLAGS := -DANDROID_NDK \
                -DDISABLE_IMPORTGL

# Hmm, best make include dir and <amju.h> first!
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Build/amjulib/include 

LOCAL_SRC_FILES := ../../../Source/AmjuGL/AmjuGL.cpp

include $(BUILD_STATIC_LIBRARY)
