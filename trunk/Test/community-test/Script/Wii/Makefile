ifeq ($(strip $(AMJULIB_WII_SCRIPT_PATH)),)
$(error "Please set AMJULIB_WII_SCRIPT_PATH in your environment.")
endif

APP		:=	community-test

# Urgh, build dir can't be same as this dir, e.g. "Wii" for both. 
# There's a check to see if we are in the build dir in GenMakefile which needs fixing
BUILD		:=	../../../../Build/$(APP)/WiiBuild
TARGET		:=	$(BUILD)/boot
AMJU      	:= 	../../../../Source

include $(AMJULIB_WII_SCRIPT_PATH)/AmjuLibDirs

SOURCES         :=      ../../Source $(AMJU_LIB_DIRS)

include $(AMJULIB_WII_SCRIPT_PATH)/GenMakefile


