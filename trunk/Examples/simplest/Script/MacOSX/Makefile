ifeq ($(strip $(AMJULIB_MAC_SCRIPT_PATH)),)
$(error "Please set AMJULIB_MAC_SCRIPT_PATH in your environment.")
endif

APP		:=	simplest

BUILD		:=	../../../../Build/$(APP)/Mac
TARGET		:=	$(BUILD)/$(APP)
LIBDIRS   	:= 	$(AMJULIB_MAC_SCRIPT_PATH)/../../Releases/amjulib-v.0.3-macosx
SOURCES		:=	../../Source
RUNDIR 		:=	$(BUILD)

LIBS	:= \
	$(AMJULIB_MAC_SCRIPT_PATH)/../../Build/Mac/amjulib.a \
	-framework OpenGL \
	-framework GLUT \
	-framework Carbon \
	-lstdc++ -lm 

include $(AMJULIB_MAC_SCRIPT_PATH)/GenMakefile

run:
	cp $(TARGET) $(RUNDIR); \
	cd $(RUNDIR); \
	./$(APP)

