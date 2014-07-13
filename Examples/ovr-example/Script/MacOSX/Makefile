ifeq ($(strip $(AMJULIB_MAC_SCRIPT_PATH)),)
$(error "Please set AMJULIB_MAC_SCRIPT_PATH in your environment.")
endif

APP		:=	ovr-example

LIBDIRS         :=      $(AMJULIB_MAC_SCRIPT_PATH)/../../Releases/amjulib-v.0.4-macosx
#LIBDIRS         :=      $(AMJULIB_MAC_SCRIPT_PATH)/../../Build/amjulib
BUILD		:=	../../../../Build/$(APP)/Mac
TARGET		:=	$(BUILD)/$(APP)
RUNDIR 		:=	$(BUILD)
SOURCES		:=	../../Source  ../../../../../../OculusSDK/LibOVR/Include 

LIBS	:= \
	/Users/jay/OculusSDK/LibOVR/Lib/MacOS/Debug/libovr.a \
	-lamju \
	-framework OpenGL \
	-framework GLUT \
	-framework Carbon \
	-framework IOKit \
	-lstdc++ -lm 

include $(AMJULIB_MAC_SCRIPT_PATH)/GenMakefile

run:
	cp -R ../../Assets/* $(RUNDIR); \
	cd $(RUNDIR); \
	./$(APP)

