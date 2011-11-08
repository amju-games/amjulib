
ifeq ($(strip $(AMJULIB_IPHONE_SCRIPT_PATH)),)
$(error "Please set AMJULIB_IPHONE_SCRIPT_PATH in your environment. ")
endif

EXTRA_CFLAGS    :=       -g -O0 -D_DEBUG 

BUILD		:=	../../Build/iPhoneBuild
TARGET		:=	$(BUILD)/amjulib.a
AMJU      := ../../../amjulib/Source
SOURCES := ../../Source \
  $(AMJU)/AmjuGL \
  $(AMJU)/AmjuGLOpenGL \
  $(AMJU)/Clip \
  $(AMJU)/Collision \
  $(AMJU)/Community \
  $(AMJU)/Events \
  $(AMJU)/FileUtils \
  $(AMJU)/Font \
  $(AMJU)/Game \
  $(AMJU)/Geom \
  $(AMJU)/Geom/Mgc \
  $(AMJU)/GeomDraw \
  $(AMJU)/GeomLoad \
  $(AMJU)/Gui \
  $(AMJU)/Image \
  $(AMJU)/Image/Png \
  $(AMJU)/Localise \
  $(AMJU)/Md2 \
  $(AMJU)/Mem \
  $(AMJU)/Network \
  $(AMJU)/Obj \
  $(AMJU)/Resource \
  $(AMJU)/Scene \
  $(AMJU)/Sound \
  $(AMJU)/SoundBass \
  $(AMJU)/Strings \
  $(AMJU)/Thread \
  $(AMJU)/TimeUtils \
  $(AMJU)/Utils \
  $(AMJU)/Utils\Xml \
  $(AMJU)/Utils/Zlib

include $(AMJULIB_IPHONE_SCRIPT_PATH)/GenMakefile
  

$(OUTPUT): $(OFILES)
	$(BUILDSTATICLIB) $(OFILES)

