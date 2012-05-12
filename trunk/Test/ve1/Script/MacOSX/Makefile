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
DATADIR		:=	$(RUNDIR)/Data

LIBS	:= \
	$(AMJULIB_MAC_SCRIPT_PATH)/../../Build/Mac/amjulib.a \
	-framework OpenGL \
	-framework GLUT \
	-framework Carbon \
	-lstdc++ -lm 

include $(AMJULIB_MAC_SCRIPT_PATH)/GenMakefile

run_old:
	mkdir $(RUNDIR); \
	mkdir $(DATADIR); \
	mkdir $(DATADIR)/font2d; \
	mkdir $(DATADIR)/font3d; \
	cp $(TARGET) $(RUNDIR); \
	cp -R $(ASSETSDIR)/* $(DATADIR); \
	cp $(FONT2D)/* $(DATADIR)/font2d/; \
	cp $(FONT3D)/* $(DATADIR)/font3d/; \
	cd $(RUNDIR); \
	./$(APP)

run:
	cp $(TARGET) $(RUNDIR); \
	cp -R $(ASSETSDIR)/* $(DATADIR); \
	cd $(RUNDIR); \
	echo run | gdb $(APP); \
	cd -

runclean:
	rm $(RUNDIR)/* ; make run

