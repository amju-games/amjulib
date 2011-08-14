ifeq ($(strip $(AMJULIB_IPHONE_SCRIPT_PATH)),)
$(error "Please set AMJULIB_IPHONE_SCRIPT_PATH in your environment.")
endif

APP		:=	community-test
NICE_NAME	:=	Amju Community

BUILD		:=	../../../../Build/$(APP)/iPhoneBuild
TARGET		:=	$(BUILD)/$(APP)
AMJU      	:= 	../../../../Source
SOURCES		:=	../../Source
RUNDIR 		:=	../../Assets

LIBS	:= \
	$(AMJULIB_IPHONE_SCRIPT_PATH)/../../Build/iPhoneBuild/amjulib.a \
	-framework UIKit \
	-framework Foundation \
        -framework OpenGLES \
        -framework QuartzCore \
        -lbassmidi \
        -lbass \
        -framework AudioToolbox \
        -framework CFNetwork \
        -framework SystemConfiguration \
        -lstdc++ -lm

include $(AMJULIB_IPHONE_SCRIPT_PATH)/GenMakefile


setup:
        mkdir -p "$(APP_DIR)"; \
        cp $(TARGET) "$(APP_DIR)/$(NICE_NAME)"; \
        cp $(AMJULIB_IPHONE_SCRIPT_PATH)/../../Assets/iPhone/AmjuWW.xcent "$(APP_DIR)"; \
        cp $(AMJULIB_IPHONE_SCRIPT_PATH)/../../Assets/iPhone/Info.plist "$(APP_DIR)"; \
        cp $(AMJULIB_IPHONE_SCRIPT_PATH)/../../Assets/iPhone/MainWindow.nib "$(APP_DIR)"
        cp $(AMJULIB_IPHONE_SCRIPT_PATH)/../../Assets/iPhone/PkgInfo "$(APP_DIR)"; \
        cp ../../Assets/iphone/icon.png "$(APP_DIR)"; 


run:
	cp $(TARGET) $(RUNDIR); \
	cd $(RUNDIR); \
	./$(APP)

