# Make release of amjulib for WII
export AMJU_BUILD_ARCH=Wii
#export AMJU_MACHDEP='-arch i386 -DMACOSX -DXP_MACOSX -D__i386__ -DAMJU_USE_BASS -DAMJU_USE_CURL'

export PLAT=wii
export VERSION=`../MakeVersionString.pl ../../Source/Version.h`
export DEST_DIR=../../Releases/amjulib-$VERSION-$PLAT
echo MAKING RELEASE FOR $DEST_DIR

export LIB=$DEST_DIR/lib/
export INC=$DEST_DIR/include/
export SRC=../../Source/

# Create release dir structure
mkdir -p $LIB
mkdir -p $INC

rm $INC/*
rm $LIB/*

# Copy headers required by client code to include folder
cp $SRC/Version.h $INC
cp $SRC/AmjuGL/*.h $INC
cp $SRC/AmjuGLGCube/*.h $INC
cp $SRC/Collision/*.h $INC
cp $SRC/Community/*.h $INC
cp $SRC/Events/*.h $INC
cp $SRC/EventsWii/*.h $INC
cp $SRC/FileUtils/*.h $INC
cp $SRC/Font/*.h $INC
cp $SRC/Game/*.h $INC
cp $SRC/Geom/*.h $INC
cp $SRC/GeomDraw/*.h $INC
cp $SRC/GeomLoad/*.h $INC
cp $SRC/Gui/*.h $INC
cp $SRC/Image/*.h $INC
cp $SRC/Localise/*.h $INC
cp $SRC/Lua/*.h $INC
cp $SRC/Mem/*.h $INC
cp $SRC/Main/*.h $INC
cp $SRC/Md2/Md2Model.h $INC
cp $SRC/Network/*.h $INC
cp $SRC/Obj/*.h $INC
cp $SRC/Resource/*.h $INC
cp $SRC/Scene/*.h $INC
cp $SRC/Sound/*.h $INC
cp $SRC/SoundWii/*.h $INC
cp $SRC/Strings/*.h $INC
cp $SRC/Thread/*.h $INC
cp $SRC/TimeUtils/*.h $INC
cp $SRC/Utils/*.h $INC
cp $SRC/Utils/Xml/*.h $INC

perl ../CreateHeaderFile.pl $INC > $INC/amju.h

# Build (release mode)
export EXTRA_CFLAGS='-g -O2 -DNDEBUG'
make clean
make

# Copy .lib binary to lib folder
cp ../../Build/WiiBuild/libamju.a $LIB/libamju.a

# Make documentation and copy to docs folder

# Copy other stuff, e.g. examples


# Copy readme


