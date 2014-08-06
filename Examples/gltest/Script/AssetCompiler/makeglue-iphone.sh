# Create glue file

export PLATFORM=iphone
export THIS_DIR=`pwd`
export TOP_DIR=$THIS_DIR/../../../..
export COMPILED_ASSETS=$TOP_DIR/Build/gltest/CompiledAssets
export DEST_DIR=$COMPILED_ASSETS/$PLATFORM
export SRC_DIR=../../Assets
export GLUE_EXE=$THIS_DIR/glue
export GLUE_FILE=$DEST_DIR/../data-$PLATFORM.glue

rm -rf $DEST_DIR
mkdir -p $DEST_DIR
mkdir -p $DEST_DIR/font2d/
mkdir -p $DEST_DIR/Shaders/gles

cp $SRC_DIR/*.obj $DEST_DIR
cp $SRC_DIR/*.png $DEST_DIR
cp $SRC_DIR/*.txt $DEST_DIR
cp $SRC_DIR/Shaders/gles/*.txt $DEST_DIR/Shaders/gles
cp $SRC_DIR/font2d/*.png $DEST_DIR/font2d/
cp $SRC_DIR/font2d/*.txt $DEST_DIR/font2d/

cd $DEST_DIR

# Create empty glue file
$GLUE_EXE -c $GLUE_FILE

for f in *.txt *.png *.obj font2d/* Shaders/gles/*
do
    echo "Adding file: " $f
    $GLUE_EXE -a $GLUE_FILE $f
done

# Verify glue file contents
$GLUE_EXE -d $GLUE_FILE

cd $THIS_DIR

