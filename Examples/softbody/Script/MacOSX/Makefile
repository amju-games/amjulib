ifeq ($(strip $(AMJULIB_MAC_SCRIPT_PATH)),)
$(error "Please set AMJULIB_MAC_SCRIPT_PATH in your environment.")
endif

APP		:=	softbody

LIBDIRS         :=      $(AMJULIB_MAC_SCRIPT_PATH)/../../Releases/amjulib-v.0.4-macosx
//LIBDIRS         :=      $(AMJULIB_MAC_SCRIPT_PATH)/../../Build/amjulib
BUILD		:=	../../../../Build/$(APP)/Mac
TARGET		:=	$(BUILD)/$(APP)
SOURCES		:=	../../Source
RUNDIR 		:=	$(BUILD)
EXTRA_CFLAGS	:= 	-DAMJU_USE_GLUT

LIBS	:= \
	-lamju \
	-framework OpenGL \
	-framework GLUT \
	-framework Carbon \
	-lstdc++ -lm 

include $(AMJULIB_MAC_SCRIPT_PATH)/GenMakefile

run:
	cp -R ../../Assets/* $(RUNDIR); \
	cd $(RUNDIR); \
	./$(APP)

