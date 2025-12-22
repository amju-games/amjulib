# Hello, SOURCE me to set vars for amjulib makefiles
# Like this:
# . ./setup_make_vars.sh

export AMJU_BUILD_ARCH=Mac
export AMJU_MACHDEP='-DMACOSX -DXP_MACOSX -DAMJU_USE_BASS_MIDI -DAMJU_USE_BASS -DAMJU_USE_CURL -DUSE_SHADOW_MAP_OPENGL_2 -DGL_SILENCE_DEPRECATION'

export AMJULIB_MAC_SCRIPT_PATH=`pwd`
