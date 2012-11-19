ifeq ($(strip $(AMJULIB_MAC_SCRIPT_PATH)),)
$(error "Please set AMJULIB_MAC_SCRIPT_PATH in your environment.")
endif

VERSION		:=	0.5

APP		:=	ve1
APPNAME		:=	"Crash Site.app"

BUILD		:=	../../../../Build/$(APP)/$(AMJU_BUILD_ARCH)
TARGET		:=	$(BUILD)/$(APP)
AMJU      	:= 	../../../../Source
SOURCES		:=	../../Source
ASSETSDIR 	:=	../../Assets
# Shared GUI assets
GUIASSETSDIR 	:=	../../../../Assets/gui
# These are the shared font assets, not just for this project
FONT2D		:=	../../../../Assets/font2d
FONT3D		:=	../../../../Assets/font3d
RUNDIR		:=	$(BUILD)/$(APPNAME)/Contents/MacOS
DATADIR		:=	$(RUNDIR)/Data

LIBS	:= \
	$(AMJULIB_MAC_SCRIPT_PATH)/../../Build/$(AMJU_BUILD_ARCH)/amjulib.a \
        $(AMJULIB_MAC_SCRIPT_PATH)/../../Source/SoundBass/Bass2.4/Macosx/mp3-free/libbass.dylib \
        $(AMJULIB_MAC_SCRIPT_PATH)/../../Source/SoundBass/Bass2.4/Macosx/libbassmidi.dylib \
	-framework OpenGL \
	-framework GLUT \
	-framework Carbon \
	-lcurl \
	-lstdc++ -lm 

include $(AMJULIB_MAC_SCRIPT_PATH)/GenMakefile

run:
	cp $(TARGET) $(RUNDIR); \
	cp special.cfg $(BUILD); \
	cd $(RUNDIR); \
	./$(APP)

# NB To be sure you have all up-to-date assets, do a make release, then make run


runclean:
	rm $(RUNDIR)/* ; make run


release:
	rm -rf $(BUILD)/$(APPNAME); \
	cp -R $(APPNAME) $(BUILD)/; \
	mkdir -p $(BUILD)/$(APPNAME)/Contents/Resources/Data; \
	mkdir -p $(BUILD)/$(APPNAME)/Contents/Resources/Data/font2d; \
	mkdir -p $(BUILD)/$(APPNAME)/Contents/Resources/Data/font3d; \
	cp -R $(ASSETSDIR)/* $(BUILD)/$(APPNAME)/Contents/Resources/Data/; \
	cp -R $(GUIASSETSDIR)/* $(BUILD)/$(APPNAME)/Contents/Resources/Data/; \
	cp -R $(FONT2D)/* $(BUILD)/$(APPNAME)/Contents/Resources/Data/font2d; \
	cp -R $(FONT3D)/* $(BUILD)/$(APPNAME)/Contents/Resources/Data/font3d; \
        cp $(AMJU)/SoundBass/Bass2.4/Macosx/libbassmidi.dylib $(BUILD)/$(APPNAME)/Contents/MacOS/; \
        cp $(AMJU)/SoundBass/Bass2.4/Macosx/mp3-free/libbass.dylib $(BUILD)/$(APPNAME)/Contents/MacOS/ ; \
	cp $(TARGET) $(BUILD)/$(APPNAME)/Contents/MacOS/"Crash Site"; 



