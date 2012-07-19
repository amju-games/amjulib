ifeq ($(strip $(AMJULIB_MAC_SCRIPT_PATH)),)
$(error "Please set AMJULIB_MAC_SCRIPT_PATH in your environment.")
endif

BUILD		:=	../../../../Build/obj2bin/Mac
TARGET		:=	$(BUILD)/obj2bin
AMJU      	:= ../../../../Source
SOURCES		:=	../../Source

LIBS	:= \
	$(AMJULIB_MAC_SCRIPT_PATH)/../../Build/Mac/amjulib.a \
	-framework OpenGL \
	-lstdc++ -lm 

include $(AMJULIB_MAC_SCRIPT_PATH)/GenMakefile

test:	
	$(TARGET) _1_.obj _1_output.obj

