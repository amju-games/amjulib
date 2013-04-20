# Make binary versions of all .obj files

export RESTORE_DIR=`pwd`
export TOP_DIR=$RESTORE_DIR/../..
export DEST_DIR=$TOP_DIR/Build/CompiledAssets/wii
export OBJ2BIN=$RESTORE_DIR/obj2bin
export SRC_DIR=$TOP_DIR/Assets

mkdir -p $DEST_DIR

cd $SRC_DIR


for f in *.obj
do
    echo "Found this file: " $f
    $OBJ2BIN $f $DEST_DIR/$f
done


cd $RESTORE_DIR

