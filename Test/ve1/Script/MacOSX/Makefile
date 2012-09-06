ifeq ($(strip $(AMJULIB_MAC_SCRIPT_PATH)),)
$(error "Please set AMJULIB_MAC_SCRIPT_PATH in your environment.")
endif

APP		:=	ve1

VERSION		:=	0.5

BUILD		:=	../../../../Build/$(APP)/$(AMJU_BUILD_ARCH)
TARGET		:=	$(BUILD)/$(APP)
AMJU      	:= 	../../../../Source
SOURCES		:=	../../Source
ASSETSDIR 	:=	../../Assets
# These are the shared font assets, not just for this project
FONT2D		:=	../../../../Assets/font2d
FONT3D		:=	../../../../Assets/font3d
RUNDIR		:=	$(BUILD)/"My Game.app"/Contents/MacOS
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
	rm -rf $(BUILD)/"My Game.app"; \
	cp -R "My Game.app" $(BUILD)/; \
	mkdir -p $(BUILD)/"My Game.app"/Contents/Resources/Data; \
	mkdir -p $(BUILD)/"My Game.app"/Contents/Resources/Data/font2d; \
	mkdir -p $(BUILD)/"My Game.app"/Contents/Resources/Data/font3d; \
	cp -R $(ASSETSDIR)/* $(BUILD)/"My Game.app"/Contents/Resources/Data/; \
	cp -R $(FONT2D)/* $(BUILD)/"My Game.app"/Contents/Resources/Data/font2d; \
	cp -R $(FONT3D)/* $(BUILD)/"My Game.app"/Contents/Resources/Data/font3d; \
        cp $(AMJU)/SoundBass/Bass2.4/Macosx/libbassmidi.dylib $(BUILD)/"My Game.app"/Contents/MacOS/; \
        cp $(AMJU)/SoundBass/Bass2.4/Macosx/mp3-free/libbass.dylib $(BUILD)/"My Game.app"/Contents/MacOS/ ; \
	cp $(TARGET) $(BUILD)/"My Game.app"/Contents/MacOS/"My Game"; 


# Do a make zip to create a zip file for publishing on web site

zip:
	make release; \
	strip $(BUILD)/"My Game.app"/Contents/MacOS/"My Game"; \
	cd $(BUILD); \
	rm "My Game.zip"; \
	zip -9 "My Game.zip" "My Game.app"; \
	zip -9 "My Game.zip" "My Game.app/Contents"; \
	zip -9 "My Game.zip" "My Game.app/Contents/Info.plist"; \
	zip -9 "My Game.zip" "My Game.app/Contents/MacOS/My Game"; \
	zip -9 "My Game.zip" "My Game.app/Contents/Resources"; \
	zip -9 -r "My Game.zip" "My Game.app/Contents/Resources/Data"; \
	zip -9 "My Game.zip" "My Game.app/Contents/Resources/MyGame.icns"; \
	zip -9 "My Game.zip" "My Game.app/Contents/Resources/English.lproj"; \
	zip -9 "My Game.zip" "My Game.app/Contents/Resources/English.lproj/InfoPlist.strings"; \
	mv "My Game.zip" "MyGame-v."$(VERSION)".zip"



