ifeq ($(strip $(AMJULIB_MAC_SCRIPT_PATH)),)
$(error "Please set AMJULIB_MAC_SCRIPT_PATH in your environment.")
endif

BUILD		:=	../../../../Build/obj2bin/Mac
TARGET		:=	$(BUILD)/obj2bin
AMJU      	:= ../../../../Source
SOURCES		:=	../../Source
RUNDIR		:=	../../TestData

LIBS	:= \
	$(AMJULIB_MAC_SCRIPT_PATH)/../../Build/Mac/amjulib.a \
	-framework Carbon \
	-framework OpenGL \
	-lstdc++ -lm 

include $(AMJULIB_MAC_SCRIPT_PATH)/GenMakefile

test:	
	cp $(TARGET) $(RUNDIR); \
	cd $(RUNDIR); \
	./obj2bin field_2x2.obj test_output.objb

