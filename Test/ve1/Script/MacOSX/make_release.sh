export VERSION=0.2

export AMJU_BUILD_ARCH=Mac-i386
export AMJU_MACHDEP='-arch i386 -DMACOSX -DXP_MACOSX -D__i386__'
pushd ../../../../Script/MacOSX
make
popd
make

export AMJU_BUILD_ARCH=Mac-ppc
export AMJU_MACHDEP='-arch ppc -DMACOSX -DXP_MACOSX'
pushd ../../../../Script/MacOSX
make
popd
make

# TODO Make fat binary
export BUILD=../../../../Build/ve1/

lipo -create $BUILD/Mac-i386/ve1 $BUILD/Mac-ppc/ve1 -output $BUILD/ve1.univ
strip ve1.univ

# Make app bundle, put universal binary in 

export ASSETSDIR=../../Assets
# These are the shared font assets, not just for this project
export FONT2D=../../../../Assets/font2d
export FONT3D=../../../../Assets/font3d

rm -rf $BUILD/"My Game.app"
cp -R "My Game.app" $BUILD
mkdir -p $BUILD/"My Game.app"/Contents/Resources/Data
mkdir -p $BUILD/"My Game.app"/Contents/Resources/Data/font2d
mkdir -p $BUILD/"My Game.app"/Contents/Resources/Data/font3d
cp -R $ASSETSDIR/* $BUILD/"My Game.app"/Contents/Resources/Data/
cp -R $FONT2D/* $BUILD/"My Game.app"/Contents/Resources/Data/font2d
cp -R $FONT3D/* $BUILD/"My Game.app"/Contents/Resources/Data/font3d
cp $BUILD/ve1.univ $BUILD/"My Game.app"/Contents/MacOS/"My Game"
cd $BUILD

# Zip bundle
rm "My Game.zip"
zip -9 "My Game.zip" "My Game.app"
zip -9 "My Game.zip" "My Game.app/Contents"
zip -9 "My Game.zip" "My Game.app/Contents/Info.plist"
zip -9 "My Game.zip" "My Game.app/Contents/MacOS/My Game"
zip -9 "My Game.zip" "My Game.app/Contents/Resources"
zip -9 -r "My Game.zip" "My Game.app/Contents/Resources/Data"
zip -9 "My Game.zip" "My Game.app/Contents/Resources/MyGame.icns"
zip -9 "My Game.zip" "My Game.app/Contents/Resources/English.lproj"
zip -9 "My Game.zip" "My Game.app/Contents/Resources/English.lproj/InfoPlist.strings"
mv "My Game.zip" "MyGame-v."$VERSION".zip"


