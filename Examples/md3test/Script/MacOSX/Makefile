ifeq ($(strip $(AMJULIB_MAC_SCRIPT_PATH)),)
$(error "Please set AMJULIB_MAC_SCRIPT_PATH in your environment.")
endif

APP		:=	md3test

LIBDIRS         :=      $(AMJULIB_MAC_SCRIPT_PATH)/../../Releases/amjulib-v.0.5-macosx  $(AMJULIB_MAC_SCRIPT_PATH)/../../3rdPartyLibs/LibOVR-0.3.2/MacOSX

BUILD		:=	../../../../Build/$(APP)/Mac
TARGET		:=	$(BUILD)/$(APP)
#AMJU      	:= 	../../../../Source
SOURCES		:=	../../Source
RUNDIR 		:=	../../Assets
#RUNDIR 		:=	/Users/jay/md3test/models/players/

LIBS	:= \
	-lamju \
	-framework OpenGL \
	-framework GLUT \
	-framework Carbon \
	-lstdc++ -lm 

include $(AMJULIB_MAC_SCRIPT_PATH)/GenMakefile

run:
	cp $(TARGET) $(RUNDIR); \
	cd $(RUNDIR); \
	./$(APP)

