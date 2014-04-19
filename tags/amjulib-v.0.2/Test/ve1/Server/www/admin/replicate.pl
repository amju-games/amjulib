#!/usr/bin/perl -w

#############################################
## REPLICATE 
#############################################

require "common.pl";

#use File::Copy;
#use File::Copy::Recursive;

#my_connect();

starthere();
print "<img src=\"../mygame.png\" align=center> <p style=\"font-family:Arial\">";

print "<b>Replicating environment...</b><br>\n";

sub replicate();

replicate();

#print "That's it!\n";

##### end of main function

sub replicate()
{
  my $src = param('src') or die "Expected source env name";
  my $dest = param('dest') or die "Expected dest env name";

  print "Source: $src, dest: $dest<br>\n";

  # Make src dir if not exists
  if (! -d "../../../$dest")
  {
    mkdir "../../../$dest" or die "Failed to create dest dir: $!";
  }

  # Copy dir contents from src to dir
  #copy ('../../../$src/*', '../../../$dest/') or die "Failed to copy: $!";  

  system("cp -R ../../../$src/* ../../../$dest/");

  print $!;
}


