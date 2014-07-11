# Create glue file

export PLATFORM=iphone
export THIS_DIR=`pwd`
export TOP_DIR=$THIS_DIR/../../../..
export COMPILED_ASSETS=$TOP_DIR/Build/vr-example/CompiledAssets
export DEST_DIR=$COMPILED_ASSETS/$PLATFORM
export SRC_DIR=../../Assets
export GLUE_EXE=$THIS_DIR/glue
export GLUE_FILE=$DEST_DIR/../data-$PLATFORM.glue

rm -rf $DEST_DIR
mkdir -p $DEST_DIR
mkdir -p $DEST_DIR/font2d/
mkdir -p $DEST_DIR/fire_temple

cp $SRC_DIR/*.png $DEST_DIR
cp $SRC_DIR/*.txt $DEST_DIR
#cp $SRC_DIR/Shaders/gles/*.txt $DEST_DIR/Shaders/gles

cp $SRC_DIR/fire_temple/*.png $DEST_DIR/
cp $SRC_DIR/fire_temple/*.mtl $DEST_DIR/
cp $SRC_DIR/fire_temple/*.obj $DEST_DIR/

cp $SRC_DIR/font2d/*.png $DEST_DIR/font2d/
cp $SRC_DIR/font2d/*.txt $DEST_DIR/font2d/

cd $DEST_DIR

# Create empty glue file
$GLUE_EXE -c $GLUE_FILE

for f in *.txt *.png font2d/* *.mtl *.obj
do
    echo "Adding file: " $f
    $GLUE_EXE -a $GLUE_FILE $f
done

# Verify glue file contents
$GLUE_EXE -d $GLUE_FILE

cd $THIS_DIR

