# Create glue file

export PLATFORM=iphone
export THIS_DIR=`pwd`
export TOP_DIR=$THIS_DIR/../..
export COMPILED_ASSETS=$TOP_DIR/Build/CompiledAssets
export DEST_DIR=$COMPILED_ASSETS/$PLATFORM
export SRC_DIR=$TOP_DIR/Assets
export GLUE_EXE=$THIS_DIR/glue
export GLUE_FILE=$DEST_DIR/../data-$PLATFORM.glue

rm -rf $DEST_DIR
mkdir -p $DEST_DIR

# use png, not bmp
cp $SRC_DIR/*.png $DEST_DIR
cp $SRC_DIR/*.txt $DEST_DIR
cp $SRC_DIR/*.text $DEST_DIR
cp $SRC_DIR/*.attr $DEST_DIR
cp $SRC_DIR/*.bill $DEST_DIR
cp $SRC_DIR/*.move $DEST_DIR
cp $SRC_DIR/*.room $DEST_DIR
cp $SRC_DIR/*.lite $DEST_DIR
cp $SRC_DIR/*.ini $DEST_DIR
cp $SRC_DIR/*.amju2 $DEST_DIR
cp $SRC_DIR/*.comp $DEST_DIR
cp $SRC_DIR/*.md2 $DEST_DIR
#cp $SRC_DIR/*.obj $DEST_DIR
#cp $SRC_DIR/*.mtl $DEST_DIR

cp $SRC_DIR/*.mod $DEST_DIR
cp $SRC_DIR/*.wav $DEST_DIR

# TODO Convert text files to binary
. makeobjsbin-iphone.sh


cd $DEST_DIR

# Create empty glue file
$GLUE_EXE -c $GLUE_FILE

for f in *.txt *.text *.attr *.bill *.move *.room *.lite *.ini *.amju2 *.md2 *.png *.obj *.comp
do
    echo "Adding file: " $f
    $GLUE_EXE -a $GLUE_FILE $f
done

# Verify glue file contents
$GLUE_EXE -d $GLUE_FILE

cd $THIS_DIR

