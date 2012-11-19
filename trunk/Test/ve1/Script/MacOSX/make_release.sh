export VERSION=0.5

export APPNAME="Crash Site.app"
export ZIPNAME="CrashSite.zip"

# Make amjulib
export AMJU_BUILD_ARCH=Mac-i386
export AMJU_MACHDEP='-arch i386 -DMACOSX -DXP_MACOSX -D__i386__'
pushd ../../../../Script/MacOSX
make
popd

# Now make the game, linking to amjulib
make

# No ppc build for now

#export AMJU_BUILD_ARCH=Mac-ppc
#export AMJU_MACHDEP='-arch ppc -DMACOSX -DXP_MACOSX'
#pushd ../../../../Script/MacOSX
#make
#popd

#make

export BUILD=../../../../Build/ve1/

# TODO Make fat binary like this if ppc and intel
#lipo -create $BUILD/Mac-i386/ve1 $BUILD/Mac-ppc/ve1 -output $BUILD/ve1.univ

# No ppc, not fat binary
cp $BUILD/Mac-i386/ve1 $BUILD/ve1.univ

strip $BUILD/ve1.univ

# Make app bundle, copy in executable and assets

export ASSETSDIR=../../Assets
# These are the shared font assets, not just for this project
export FONT2D=../../../../Assets/font2d
export FONT3D=../../../../Assets/font3d

rm -rf $BUILD/"$APPNAME"
cp -R "$APPNAME" $BUILD
mkdir -p $BUILD/"$APPNAME"/Contents/Resources/Data
mkdir -p $BUILD/"$APPNAME"/Contents/Resources/Data/font2d
mkdir -p $BUILD/"$APPNAME"/Contents/Resources/Data/font3d
cp -R $ASSETSDIR/* $BUILD/"$APPNAME"/Contents/Resources/Data/
cp -R $FONT2D/* $BUILD/"$APPNAME"/Contents/Resources/Data/font2d
cp -R $FONT3D/* $BUILD/"$APPNAME"/Contents/Resources/Data/font3d
cp $BUILD/ve1.univ $BUILD/"$APPNAME"/Contents/MacOS/"Crash Site"
cp ../../../../Source/SoundBass/Bass2.4/Macosx/libbassmidi.dylib $BUILD/"$APPNAME"/Contents/MacOS/
cp ../../../../Source/SoundBass/Bass2.4/Macosx/mp3-free/libbass.dylib $BUILD/"$APPNAME"/Contents/MacOS/
cd $BUILD

# Zip bundle
rm "My Game.zip"
zip -9 "$ZIPNAME" "$APPNAME"
zip -9 "$ZIPNAME" "$APPNAME/Contents"
zip -9 "$ZIPNAME" "$APPNAME/Contents/Info.plist"
zip -9 "$ZIPNAME" "$APPNAME/Contents/MacOS/Crash Site"
zip -9 "$ZIPNAME" "$APPNAME/Contents/MacOS/libbass.dylib"
zip -9 "$ZIPNAME" "$APPNAME/Contents/MacOS/libbassmidi.dylib"
zip -9 "$ZIPNAME" "$APPNAME/Contents/Resources"
zip -9 -r "$ZIPNAME" "$APPNAME/Contents/Resources/Data"
zip -9 "$ZIPNAME" "$APPNAME/Contents/Resources/MyGame.icns"
zip -9 "$ZIPNAME" "$APPNAME/Contents/Resources/English.lproj"
zip -9 "$ZIPNAME" "$APPNAME/Contents/Resources/English.lproj/InfoPlist.strings"
mv "$ZIPNAME" "../../../../../CrashSite-v."$VERSION".zip"


