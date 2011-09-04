ifeq ($(strip $(AMJULIB_MAC_SCRIPT_PATH)),)
$(error "Please set AMJULIB_MAC_SCRIPT_PATH in your environment.")
endif

BUILD		:=	../../../../Build/bmp2png/Mac
TARGET		:=	$(BUILD)/bmp2png
AMJU      	:= 	../../../../Source
SOURCES		:=	../../Source

LIBS	:= \
	$(AMJULIB_MAC_SCRIPT_PATH)/../../Build/Mac/amjulib.a \
	-framework OpenGL \
	-lstdc++ -lm 

include $(AMJULIB_MAC_SCRIPT_PATH)/GenMakefile


