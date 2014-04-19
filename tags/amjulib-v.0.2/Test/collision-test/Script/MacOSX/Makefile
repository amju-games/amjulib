ifeq ($(strip $(AMJULIB_MAC_SCRIPT_PATH)),)
$(error "Please set AMJULIB_MAC_SCRIPT_PATH in your environment.")
endif

APP		:=	collision-test

BUILD		:=	../../../../Build/$(APP)/Mac
TARGET		:=	$(BUILD)/$(APP)
AMJU      	:= 	../../../../Source
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
	cp -R ../../Assets/* $(RUNDIR); \
	cd $(RUNDIR); \
	./$(APP)

