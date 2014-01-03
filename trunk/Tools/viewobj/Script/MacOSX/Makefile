ifeq ($(strip $(AMJULIB_MAC_SCRIPT_PATH)),)
$(error "Please set AMJULIB_MAC_SCRIPT_PATH in your environment.")
endif

APP		:=	viewobj

BUILD		:=	../../../../Build/$(APP)/Mac
TARGET		:=	$(BUILD)/$(APP)
AMJU      	:= 	../../../../Source
SOURCES		:=	../../Source
RUNDIR		:=	$(BUILD)

LIBS	:= \
	$(AMJULIB_MAC_SCRIPT_PATH)/../../Build/Mac/amjulib.a \
	-framework OpenGL \
	-framework GLUT \
	-framework Carbon \
	-lstdc++ -lm 

EXTRA_CFLAGS 	:= 	-D_DEBUG

include $(AMJULIB_MAC_SCRIPT_PATH)/GenMakefile

run:
	mkdir $(RUNDIR); \
	mkdir $(RUNDIR)/font2d ; \
	cp $(TARGET) $(RUNDIR); \
	cp -R ../../Assets/* $(RUNDIR); \
	cp -R ../../../../Assets/font2d/* $(RUNDIR)/font2d ; \
	cp -R ../../../../Assets/gui/*.png $(RUNDIR); \
	cd $(RUNDIR); \
	./$(APP)


