ifeq ($(strip $(AMJULIB_MAC_SCRIPT_PATH)),)
$(error "Please set AMJULIB_MAC_SCRIPT_PATH in your environment.")
endif

APP		:=	ve1FltkUpdater

BUILD		:=	../../../../../../Build/$(APP)/Mac
TARGET		:=	$(BUILD)/$(APP)
AMJU      	:= 	../../../../../../Source
SOURCES		:=	../../Source ../../../Generic/Source
RUNDIR		:=	$(BUILD)

LIBS	:= \
	$(AMJULIB_MAC_SCRIPT_PATH)/../../Build/Mac/amjulib.a \
	-framework Carbon \
	-framework Cocoa \
	-lfltk -lstdc++ -lm 

include $(AMJULIB_MAC_SCRIPT_PATH)/GenMakefile

run:
	cd $(RUNDIR); \
	./$(APP)


