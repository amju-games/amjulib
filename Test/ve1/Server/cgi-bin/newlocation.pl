#!/usr/bin/perl -w

require "common.pl";
require "fileup_impl.pl";

# Save .obj mesh file in assets dir
fileup_impl("obj_filename");

fileup_impl("mtl_filename");

my $loc_id = param('loc_id');

my $path = "../assets_test/";

# Make asset list: it's these two files plus images files. Need to parse mtl file to get them.
my $assetFileNoPath = "location_" . $loc_id . "_assets.txt";
my $assetFile = $path . $assetFileNoPath;

if (!open(ASSET_FILE, ">$assetFile"))
{
  die "Can't open this file to write: $assetFile\n";
}
print ASSET_FILE "// Asset file for location $loc_id\r\n// Ver\r\n1\r\n";
print ASSET_FILE param('obj_filename') . "\r\n";
print ASSET_FILE param('mtl_filename') . "\r\n";
close (ASSET_FILE);

# Make data file: this format depends on what the Terrain class expects.
# Write version then .obj filename.
my $dataFileNoPath = "location_" . $loc_id . "_data.txt";
my $dataFile = $path . $dataFileNoPath; 

if (!open(DATA_FILE, ">$dataFile"))
{
  die "Can't open this file for writing: $dataFile\n";
}

print DATA_FILE "// Data file for location $loc_id\r\n// Ver\r\n1\r\n";
print DATA_FILE param('obj_filename') . "\r\n";
close (DATA_FILE);

# Get new object ID
my $sql = "select max(id) from object limit 1";
my $query = $dbh->prepare($sql) or die "Failed to prepare this query: $query\n";
$query->execute;
my ($obj_id) = $query->fetchrow_array;
$obj_id++;
print "New obj ID: $obj_id\n";

# Add row to object table and objectpos
$sql = "insert into object (id, type, assetfile, datafile, owner, createtime) values ($obj_id, 'terrain', '$assetFileNoPath', '$dataFileNoPath', 0, now() ) ";
insert($sql);

$sql = "insert into objectpos (id, x, y, z, loc, whenchanged) values ($obj_id, 0, 0, 0, $loc_id, now())";
insert($sql);

