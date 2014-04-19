# Create glue file

export PLATFORM=iphone
export THIS_DIR=`pwd`
export TOP_DIR=$THIS_DIR/../../../..
export COMPILED_ASSETS=$TOP_DIR/Build/ve3/CompiledAssets
export DEST_DIR=$COMPILED_ASSETS/$PLATFORM
export SRC_DIR=../../Assets
export GLUE_EXE=$THIS_DIR/glue
export GLUE_FILE=$DEST_DIR/../data-$PLATFORM.glue

# These are the shared font assets, not just for this project
export GUIASSETSDIR=../../../../Assets/gui
export FONT2D=../../../../Assets/font2d

rm -rf $DEST_DIR
mkdir -p $DEST_DIR
mkdir -p $DEST_DIR/characters/2d/
mkdir -p $DEST_DIR/font2d/
mkdir -p $DEST_DIR/Sound/
mkdir -p $DEST_DIR/rooms/

cp $SRC_DIR/*.png $DEST_DIR
cp $SRC_DIR/*.txt $DEST_DIR
cp $SRC_DIR/rooms/* $DEST_DIR/rooms/
cp $SRC_DIR/characters/2d/*.png $DEST_DIR/characters/2d/
cp $SRC_DIR/font2d/*.png $DEST_DIR/font2d/
cp $SRC_DIR/font2d/*.txt $DEST_DIR/font2d/
cp $FONT2D/arial*.png $DEST_DIR/font2d/
cp $FONT2D/arial*.txt $DEST_DIR/font2d/
cp $GUIASSETSDIR/*.png $DEST_DIR
cp $GUIASSETSDIR/*.txt $DEST_DIR

# no need to have Sound separately??
cp $SRC_DIR/Sound/*.it $DEST_DIR/Sound/
cp $SRC_DIR/Sound/*.wav $DEST_DIR/Sound/
cp $SRC_DIR/Sound/*.ogg $DEST_DIR/Sound/

cd $DEST_DIR

# Create empty glue file
$GLUE_EXE -c $GLUE_FILE
$
for f in *.txt *.png characters/2d/* font2d/* rooms/* Sound/*
do
    echo "Adding file: " $f
    $GLUE_EXE -a $GLUE_FILE $f
done

# Verify glue file contents
$GLUE_EXE -d $GLUE_FILE

cd $THIS_DIR

