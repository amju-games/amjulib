ifeq ($(strip $(AMJULIB_MAC_SCRIPT_PATH)),)
$(error "Please set AMJULIB_MAC_SCRIPT_PATH in your environment.")
endif

APP		:=	ve1

BUILD		:=	../../../../Build/$(APP)/Mac
TARGET		:=	$(BUILD)/$(APP)
AMJU      	:= 	../../../../Source
SOURCES		:=	../../Source
ASSETSDIR 	:=	../../Assets
FONT2D		:=	../../../../Assets/font2d
FONT3D		:=	../../../../Assets/font3d
RUNDIR		:=	/Users/Shared/ve1

LIBS	:= \
	$(AMJULIB_MAC_SCRIPT_PATH)/../../Build/Mac/amjulib.a \
	-framework OpenGL \
	-framework GLUT \
	-framework Carbon \
	-lstdc++ -lm 

include $(AMJULIB_MAC_SCRIPT_PATH)/GenMakefile

run:
	mkdir $(RUNDIR); \
	mkdir $(RUNDIR)/font2d; \
	mkdir $(RUNDIR)/font3d; \
	cp $(TARGET) $(RUNDIR); \
	cp -R $(ASSETSDIR)/* $(RUNDIR); \
	cp $(FONT2D)/* $(RUNDIR)/font2d/; \
	cp $(FONT3D)/* $(RUNDIR)/font3d/; \
	cd $(RUNDIR); \
	./$(APP)

runclean:
	rm $(RUNDIR)/* ; make run

