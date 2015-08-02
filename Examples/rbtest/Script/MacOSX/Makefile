ifeq ($(strip $(AMJULIB_MAC_SCRIPT_PATH)),)
$(error "Please set AMJULIB_MAC_SCRIPT_PATH in your environment.")
endif

APP		:=	rbtest

BUILD		:=	../../../../Build/$(APP)/Mac
LIBDIRS         :=      $(AMJULIB_MAC_SCRIPT_PATH)/../../Releases/amjulib-v.0.5-macosx
TARGET		:=	$(BUILD)/$(APP)
SOURCES		:=	../../Source
RUNDIR 		:=	$(BUILD)

LIBS	:= \
	-lamju \
	-framework OpenGL \
	-framework GLUT \
	-framework Carbon \
	-lstdc++ -lm 

include $(AMJULIB_MAC_SCRIPT_PATH)/GenMakefile

run:
	cp -R ../../Assets/*  $(RUNDIR); \
	cd $(RUNDIR); \
	./$(APP)

