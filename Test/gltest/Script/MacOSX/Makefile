ifeq ($(strip $(AMJULIB_MAC_SCRIPT_PATH)),)
$(error "Please set AMJULIB_MAC_SCRIPT_PATH in your environment.")
endif

BUILD		:=	../../../../Build/gltest/Mac
TARGET		:=	$(BUILD)/gltest
AMJU      	:= 	../../../../Source
SOURCES		:=	../../Source
RUNDIR 		:=	../../Assets

LIBS	:= \
	$(AMJULIB_MAC_SCRIPT_PATH)/../../Build/Mac/amjulib.a \
	-framework OpenGL \
	-framework GLUT \
	-lstdc++ -lm 

include $(AMJULIB_MAC_SCRIPT_PATH)/GenMakefile

run:
	cp $(TARGET) $(RUNDIR); \
	cd $(RUNDIR); \
	./gltest

