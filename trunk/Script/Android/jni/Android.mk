# Amjulib for Android 
# Jason Colman 2014


LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := amju

# override apparently not supported?
LOCAL_CFLAGS := $(EXTRA_CFLAGS) \
		-std=c++0x -DANDROID_NDK \
		-DPVRSDK \
		-Doverride= 	

LOCAL_C_INCLUDES := \
	../../../Source/AmjuGL \
	../../../Source/AmjuGLES \
	../../../Source/Collision \
	../../../Source/Community \
	../../../Source/Events \
	../../../Source/FileUtils \
	../../../Source/Font \
	../../../Source/Game \
	../../../Source/Geom \
	../../../Source/GeomDraw\
	../../../Source/GeomLoad \
	../../../Source/Gui \
	../../../Source/Image \
	../../../Source/Image/Png \
	../../../Source/Localise \
	../../../Source/Lua \
	../../../Source/Md2 \
	../../../Source/Mem \
	../../../Source/Network \
	../../../Source/Obj \
	../../../Source/Resource \
	../../../Source/Scene \
	../../../Source/Sound \
	../../../Source/Strings \
	../../../Source/Thread \
	../../../Source/TimeUtils \
	../../../Source/Utils \
	../../../Source/Utils/Mgc \
	../../../Source/Utils/Gpc

# Create list of all .c and .cpp files in above directory list.
LOCAL_SRC_FILES := \
	$(foreach dir,$(LOCAL_C_INCLUDES),$(wildcard $(dir)/*.cpp)) \
	$(foreach dir,$(LOCAL_C_INCLUDES),$(wildcard $(dir)/*.c))

include $(BUILD_STATIC_LIBRARY)

