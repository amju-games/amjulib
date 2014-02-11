# Amjulib for Android 
# Jason Colman 2014


LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := amju

# O no, super shit, override and nullptr not supported?!
LOCAL_CFLAGS := -DANDROID_NDK \
				-DPVRSDK \
				-Doverride= \
				-Dnullptr=0

# Oh, sigh
LOCAL_C_INCLUDES := \
	../../../Source/AmjuGL \
	../../../Source/AmjuGLOpenGL \
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
	../../../Source/Utils/Mgc

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
	../../../Source/Community/DownloadReq.cpp \
	../../../Source/Events/EventListener.cpp \
	../../../Source/Events/EventPoller.cpp \
	../../../Source/Events/EventTypes.cpp \
	../../../Source/Events/GameStateListener.cpp \
	../../../Source/FileUtils/ConfigFile.cpp \
	../../../Source/FileUtils/Directory.cpp \
	../../../Source/FileUtils/Endian.cpp \
	../../../Source/FileUtils/File.cpp \
	../../../Source/FileUtils/FileImpl.cpp \
	../../../Source/FileUtils/FileImplGlue.cpp \
	../../../Source/FileUtils/FileImplMem.cpp \
	../../../Source/FileUtils/FileImplStd.cpp \
	../../../Source/FileUtils/FileSize.cpp \
	../../../Source/FileUtils/GlueFile.cpp \
	../../../Source/FileUtils/GlueFileMem.cpp \
	../../../Source/FileUtils/MkDir.cpp \
	../../../Source/FileUtils/ROConfig.cpp \
	../../../Source/FileUtils/StringsFile.cpp \
	../../../Source/FileUtils/Tokeniser.cpp \
	../../../Source/Font/Font.cpp \
	../../../Source/Font/TextureSequence.cpp \
	../../../Source/Game/Game.cpp \
	../../../Source/Game/GameObject.cpp \
	../../../Source/Game/GameState.cpp


include $(BUILD_STATIC_LIBRARY)
