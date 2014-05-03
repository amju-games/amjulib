# "Simplest" example for Android 
# Jason Colman 2014


LOCAL_PATH := $(call my-dir)


include $(CLEAR_VARS)
LOCAL_MODULE := amju-prebuilt
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../../../../Releases/amjulib-v.0.3-android/lib/armeabi/libamju.a 
include $(PREBUILT_STATIC_LIBRARY)


include $(CLEAR_VARS)

LOCAL_MODULE    := simplest

# O no, super shit, override not supported?!
LOCAL_CFLAGS := -std=c++0x -DANDROID_NDK \
				-DPVRSDK \
				-Doverride= 

LOCAL_C_INCLUDES := \
	../../../Source\
	../../../../../../amjulib/Releases/amjulib-v.0.3-android/include

# Create list of all .cpp files in above directory list.
LOCAL_SRC_FILES := \
	$(foreach dir,$(LOCAL_C_INCLUDES),$(wildcard $(dir)/*.cpp)) \
	$(foreach dir,$(LOCAL_C_INCLUDES),$(wildcard $(dir)/*.c))

#LOCAL_LDFLAGS += \
#	-L../../../../../../amjulib/Releases/amjulib-v.0.1-android/lib \
#	-L../../../../../../amjulib/Releases/amjulib-v.0.1-android/lib/armeabi-v7a

#LOCAL_STATIC_LIBRARIES := -l../../../../../../amjulib/Releases/amjulib-v.0.1-android/lib/armeabi-v7a/libamju.a

LOCAL_STATIC_LIBRARIES := amju-prebuilt

LOCAL_LDLIBS := -lz -lEGL -lGLESv2 -ldl -llog

include $(BUILD_SHARED_LIBRARY)

