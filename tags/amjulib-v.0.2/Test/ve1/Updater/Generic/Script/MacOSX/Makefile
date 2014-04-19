ifeq ($(strip $(AMJULIB_MAC_SCRIPT_PATH)),)
$(error "Please set AMJULIB_MAC_SCRIPT_PATH in your environment.")
endif

APP		:=	ve1Updater

BUILD		:=	../../../../../../Build/$(APP)/Mac
TARGET		:=	$(BUILD)/$(APP)
AMJU      	:= 	../../../../../../Source
SOURCES		:=	../../Source
RUNDIR		:=	$(BUILD)

LIBS	:= \
	$(AMJULIB_MAC_SCRIPT_PATH)/../../Build/Mac/amjulib.a \
	-framework Carbon \
	-lstdc++ -lm 

include $(AMJULIB_MAC_SCRIPT_PATH)/GenMakefile

run:
	cd $(RUNDIR); \
	./$(APP)


