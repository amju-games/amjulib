# Copy just the lib binary of amjulib for MacOSX
# Use this for quickness only if you have already built the library, or
#  maybe you want a special build

export PLAT=macosx
export VERSION=`../MakeVersionString.pl ../../Source/Version.h`
export DEST_DIR=../../Releases/amjulib-$VERSION-$PLAT
export LIB=$DEST_DIR/lib/

# Copy .lib binary to lib folder
cp ../../Build/Mac/amjulib.a $LIB/libamju.a

