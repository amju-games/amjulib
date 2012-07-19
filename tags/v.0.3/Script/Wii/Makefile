ifeq ($(strip $(AMJULIB_WII_SCRIPT_PATH)),)
$(error "Please set AMJULIB_WII_SCRIPT_PATH in your environment. ")
endif


BUILD		:=	../../Build/WiiBuild
TARGET		:=	$(BUILD)/libamju
AMJU      	:= 	../../Source

include $(AMJULIB_WII_SCRIPT_PATH)/AmjuLibDirs

SOURCES := $(AMJU_LIB_DIRS)
 

include $(AMJULIB_WII_SCRIPT_PATH)/GenMakelib
  

