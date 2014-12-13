ifeq ($(strip $(AMJULIB_MAC_SCRIPT_PATH)),)
$(error "Please set AMJULIB_MAC_SCRIPT_PATH in your environment.")
endif

APP		:=	guitest

BUILD		:=	../../../../Build/$(APP)/Mac
TARGET		:=	$(BUILD)/$(APP)
SOURCES		:=	../../Source
ASSETSDIR1 	:=	../../Assets
ASSETSDIR2 	:=	../../../../Assets/gui
RUNDIR		:=	$(BUILD)

LIBDIRS   	:= 	$(AMJULIB_MAC_SCRIPT_PATH)/../../Releases/amjulib-v.0.2-macosx

LIBS	:= \
	-lamju \
	-framework OpenGL \
	-framework GLUT \
	-framework Carbon \
	-lstdc++ -lm 

include $(AMJULIB_MAC_SCRIPT_PATH)/GenMakefile

run:
	cp $(ASSETSDIR1)/* $(RUNDIR); \
	cp $(ASSETSDIR2)/* $(RUNDIR); \
	mkdir $(RUNDIR)/font2d; \
	cp ../../../../Assets/font2d/* $(RUNDIR)/font2d/; \
	cd $(RUNDIR); \
	./$(APP)

