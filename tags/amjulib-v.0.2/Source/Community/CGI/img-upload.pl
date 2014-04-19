#!/usr/bin/perl -w

# NB Important web server note:
# The web server may be configured so that any URL containing %00
# (i.e. null byte) gives a 403 Forbidden error.
# You can turn this off by adding this line to the .htaccess file
# on the server:
#
#   secFilterForceByteRange 0 255
#
#

require "common.pl";

# Server directory where player data is saved.
# NB You need to manually mkdir this directory on the server.
my $SAVE_DIR = "../player-data/";
my $MAX_NUM_IMAGES = 1;

sub MakeImageFilename($)
{
  my $id = shift;
  return $SAVE_DIR . "image-" . $id . ".png";
}

#
# Start here
#

sub do_copy_file($ $)
{
  # File rename
  my $src = shift;
  my $dest = shift;

  # rm $dest
  unlink($dest);

  # mv $src $dest  
  rename($src, $dest);

  # TODO TEMP TEST
  print "Copy file from $src to $dest";
}


sub save_image()
{
  # Client uploads image; write to image-<id>.png 
  #  TODO other file types; more than one image per zoo

  my $playerid = param('playerid') || die ("Expected playerid");
  my $data = param('image') || die ("Expected image file data");

  my $filename = MakeImageFilename($playerid);
  # Make temp filename, so we don't overwrite a good file until we confirm
  # that the upload was successful.
#  $filename = $filename . ".tmp";

  # Write to file: ">" means Truncate
  open (MYFILE, ">$filename") || die ("Can't open file $filename");
  print "Got image data: ", length($data), " bytes. Writing to file $filename..\n";
  print MYFILE $data;
  close(MYFILE);
}


save_image();







