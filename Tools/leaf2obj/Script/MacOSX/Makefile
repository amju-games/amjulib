ifeq ($(strip $(AMJULIB_MAC_SCRIPT_PATH)),)
$(error "Please set AMJULIB_MAC_SCRIPT_PATH in your environment.")
endif

BUILD		:=	../../../../Build/leaf2obj/Mac
TARGET		:=	$(BUILD)/leaf2obj
AMJU      	:= ../../../../Source
SOURCES		:=	../../Source

LIBS	:= \
	$(AMJULIB_MAC_SCRIPT_PATH)/../../Build/Mac/amjulib.a \
	-lstdc++ -lm 

include $(AMJULIB_MAC_SCRIPT_PATH)/GenMakefile


