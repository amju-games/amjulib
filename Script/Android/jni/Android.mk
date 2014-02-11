# Amjulib for Android 
# Jason Colman 2014


LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := amju

LOCAL_CFLAGS := -DANDROID_NDK \
                -DDISABLE_IMPORTGL

# Hmm, best make include dir and <amju.h> first!
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Build/amjulib/include 

# Oh noes :-(
LOCAL_SRC_FILES := \
	../../../Source/AmjuGL/AmjuGL.cpp \
	../../../Source/AmjuGL/AmjuGLWindowInfo.cpp \
	../../../Source/AmjuGL/Colour.cpp \
	../../../Source/AmjuGL/Screen.cpp \
	../../../Source/AmjuGL/TriList.cpp


include $(BUILD_STATIC_LIBRARY)
