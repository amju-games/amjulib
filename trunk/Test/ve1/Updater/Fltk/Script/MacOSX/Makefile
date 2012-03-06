ifeq ($(strip $(AMJULIB_MAC_SCRIPT_PATH)),)
$(error "Please set AMJULIB_MAC_SCRIPT_PATH in your environment.")
endif

APP		:=	ve1FltkUpdater

VERSION		:=	0.1

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

release:
	rm -rf $(BUILD)/"My Game.app"; \
	cp -R "My Game.app" $(BUILD)/; \
        mkdir -p $(BUILD)/"My Game.app"/Contents/Resources/Data; \
        mkdir -p $(BUILD)/"My Game.app"/Contents/Resources/Data/font2d; \
        mkdir -p $(BUILD)/"My Game.app"/Contents/Resources/Data/font3d; \
        cp -R ../../../../Assets/* $(BUILD)/"My Game.app"/Contents/Resources/Data/; \
        cp -R ../../../../../../Assets/font2d/* $(BUILD)/"My Game.app"/Contents/Resources/Data/font2d; \
        cp -R ../../../../../../Assets/font3d/* $(BUILD)/"My Game.app"/Contents/Resources/Data/font3d; \
        cp $(TARGET) $(BUILD)/"My Game.app"/Contents/MacOS/"My Game"; \
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

