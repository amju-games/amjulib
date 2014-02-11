# Amjulib for Android 
# Jason Colman 2014


LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := amju

LOCAL_CFLAGS := -DANDROID_NDK \
                -DDISABLE_IMPORTGL \
				-DPVRSDK

# Hmm, best make include dir and <amju.h> first!
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Build/amjulib/include 

# Oh noes :-(
LOCAL_SRC_FILES := \
	../../../Source/AmjuGL/AmjuGL.cpp \
	../../../Source/AmjuGL/AmjuGLWindowInfo.cpp \
	../../../Source/AmjuGL/Colour.cpp \
	../../../Source/AmjuGL/Screen.cpp \
	../../../Source/AmjuGL/TriList.cpp \
	../../../Source/AmjuGLOpenGL/AmjuGL-OpenGL-Base.cpp \
	../../../Source/AmjuGLOpenGL/AmjuGL-OpenGLES.1.cpp \
	../../../Source/AmjuGLOpenGL/AmjuGL-OpenGLES.2.cpp \
	../../../Source/AmjuGLOpenGL/ES2DefaultShaders.cpp \
	../../../Source/AmjuGLOpenGL/GLShader.cpp \
	../../../Source/Collision/BruteForce.cpp \
	../../../Source/Collision/CollisionManager.cpp \
	../../../Source/Collision/CollisionMesh.cpp \
	../../../Source/Collision/SAP.cpp \
	../../../Source/Community/DownloadReq.cpp


include $(BUILD_STATIC_LIBRARY)
