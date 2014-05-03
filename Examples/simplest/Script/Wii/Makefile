ifeq ($(strip $(AMJULIB_WII_SCRIPT_PATH)),)
$(error "Please set AMJULIB_WII_SCRIPT_PATH in your environment.")
endif

BUILD		:=	../../../../Build/simplest/WiiBuild
TARGET		:=	$(BUILD)/boot
LIBDIRS   	:= 	$(AMJULIB_WII_SCRIPT_PATH)/../../Releases/amjulib-v.0.3-wii


SOURCES		:=	../../Source 

include $(AMJULIB_WII_SCRIPT_PATH)/GenMakefile


