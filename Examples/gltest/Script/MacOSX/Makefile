ifeq ($(strip $(AMJULIB_MAC_SCRIPT_PATH)),)
$(error "Please set AMJULIB_MAC_SCRIPT_PATH in your environment.")
endif

APP		:=	gltest

LIBDIRS         :=      $(AMJULIB_MAC_SCRIPT_PATH)/../../Releases/amjulib-v.0.5-macosx  $(AMJULIB_MAC_SCRIPT_PATH)/../../3rdPartyLibs/LibOVR-0.3.2/MacOSX

BUILD		:=	../../../../Build/$(APP)/Mac
TARGET		:=	$(BUILD)/$(APP)
SOURCES		:=	../../Source
RUNDIR 		:=	$(BUILD)
#EXTRA_CFLAGS	+=	-DAMJU_USE_OVR

LIBS	:= \
	-lamju \
	-framework OpenGL \
	-framework GLUT \
	-framework Carbon \
	-framework IOKit \
	-framework QuartzCore \
	-framework CoreVideo \
	-framework ApplicationServices \
	-lstdc++ -lm 
#	-lovr

include $(AMJULIB_MAC_SCRIPT_PATH)/GenMakefile

run:
	cp -R ../../Assets/* $(RUNDIR); \
	cd $(RUNDIR); \
	./$(APP)

