ifeq ($(strip $(AMJULIB_MAC_SCRIPT_PATH)),)
$(error "Please set AMJULIB_MAC_SCRIPT_PATH in your environment.")
endif

APP		:=	vr-example

LIBDIRS         :=      $(AMJULIB_MAC_SCRIPT_PATH)/../../Releases/amjulib-v.0.5-macosx  $(AMJULIB_MAC_SCRIPT_PATH)/../../3rdPartyLibs/LibOVR-0.3.2/MacOSX
BUILD		:=	../../../../Build/$(APP)/Mac
TARGET		:=	$(BUILD)/$(APP)
SOURCES		:=	../../Source
RUNDIR 		:=	$(BUILD)
EXTRA_CFLAGS	+= 	-D AMJU_USE_OVR

LIBS	:= \
	-lamju \
	-lovr \
	-framework OpenGL \
	-framework GLUT \
	-framework Carbon \
	-framework IOKit \
	-framework QuartzCore \
	-framework CoreVideo \
	-framework ApplicationServices \
	-lstdc++ -lm 

include $(AMJULIB_MAC_SCRIPT_PATH)/GenMakefile

run:
	mkdir $(RUNDIR); \
	mkdir $(RUNDIR)/Shaders; \
	mkdir $(RUNDIR)/font2d; \
	cp -R ../../Assets/Shaders/* $(RUNDIR)/Shaders; \
	cp -R ../../Assets/font2d/* $(RUNDIR)/font2d; \
	cp -R ../../Assets/fire_temple/* $(RUNDIR); \
	cp $(TARGET) $(RUNDIR); \
	cd $(RUNDIR); \
	./$(APP)

