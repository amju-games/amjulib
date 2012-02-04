ifeq ($(strip $(AMJULIB_MAC_SCRIPT_PATH)),)
$(error "Please set AMJULIB_MAC_SCRIPT_PATH in your environment.")
endif

APP		:=	viewobj

BUILD		:=	../../../../Build/$(APP)/Mac
TARGET		:=	$(BUILD)/$(APP)
AMJU      	:= ../../../../Source
SOURCES		:=	../../Source
RUNDIR		:=	~/projects/oot/scene_1/

LIBS	:= \
	$(AMJULIB_MAC_SCRIPT_PATH)/../../Build/Mac/amjulib.a \
	-framework OpenGL \
	-framework GLUT \
	-lstdc++ -lm 

EXTRA_CFLAGS 	:= 	-D_DEBUG

include $(AMJULIB_MAC_SCRIPT_PATH)/GenMakefile

run:
	mkdir $(RUNDIR); \
	cp $(TARGET) $(RUNDIR); \
	cd $(RUNDIR); \
	./$(APP)


