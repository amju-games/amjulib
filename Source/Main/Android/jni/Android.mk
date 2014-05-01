# Amju NDK/Android test
# Jason Colman 2014


LOCAL_PATH := $(call my-dir)

# Use local copy of amjulib release

include $(CLEAR_VARS)
LOCAL_MODULE := amju-prebuilt
LOCAL_SRC_FILES := ../amjulib-v.0.1-android/lib/armeabi/libamju.a 
include $(PREBUILT_STATIC_LIBRARY)


include $(CLEAR_VARS)

LOCAL_MODULE    := trythis

# O no, override not supported?!
LOCAL_CFLAGS := -DNDK_DEBUG=1 -std=c++0x -DANDROID_NDK \
				-DPVRSDK \
				-Doverride= 

LOCAL_C_INCLUDES := \
	. \
	../amjulib-v.0.1-android/include

MY_SRC_DIRS = \
	. \
	../../../amjulib/Examples/simplest/Source

# Create list of all .cpp files in above directory list.
LOCAL_SRC_FILES := \
	$(foreach dir,$(MY_SRC_DIRS),$(wildcard $(dir)/*.cpp)) \
	$(foreach dir,$(MY_SRC_DIRS),$(wildcard $(dir)/*.c)) 


LOCAL_STATIC_LIBRARIES := amju-prebuilt

LOCAL_LDLIBS := -lz -lEGL -lGLESv2 -ldl -llog

include $(BUILD_SHARED_LIBRARY)

