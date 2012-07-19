ifeq ($(strip $(AMJULIB_MAC_SCRIPT_PATH)),)
$(error "Please set AMJULIB_MAC_SCRIPT_PATH in your environment.")
endif

APP		:=	guitest

BUILD		:=	../../../../Build/$(APP)/Mac
TARGET		:=	$(BUILD)/$(APP)
AMJU      	:= 	../../../../Source
SOURCES		:=	../../Source
ASSETSDIR1 	:=	../../Assets
ASSETSDIR2 	:=	../../../../Assets/gui
RUNDIR		:=	$(BUILD)

LIBS	:= \
	$(AMJULIB_MAC_SCRIPT_PATH)/../../Build/Mac/amjulib.a \
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

