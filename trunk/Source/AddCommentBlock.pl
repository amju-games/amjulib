#!/usr/bin/perl -w

#
# Amju Games copyright (c) 2009
# Add comment block to top of C++ source and header files
#
# Arguments:
#  - Directory in which to start looking for source files:
#    we then recurse into subdirs.
#  - Optionally, "nowrite" - don't write modified file
#
# Usually run like this:
#   ./AddCommentBlock.pl ../../Code [nowrite]
#   perl AddCommentBlock.pl ../../Code [nowrite]
#

use File::Find;
use strict;

my $COMMENT_BLOCK = "/* Amju Games source code (c) Copyright 2000-2013 Jason Colman */\n\n";

sub AddHeaders($)
{
  # Read file to modify
  # -------------------

  my $fileToModify = shift; 
  if (!open(MODIFY_THIS, $fileToModify))
  {
    print "Can't open specified file to modify: $fileToModify\n";
    return;
  }

  my @lines = <MODIFY_THIS>;
  close (MODIFY_THIS);

  # Check for "/*" on top line.
  # If found, set flag. 
  # If flag is set, delete each line until
  # we find "*/". 
  # Then add new comment block to top of file.
 
  my $lineNum = 0;
  my $wasChanged = 0;
  my $foundCommentBlock = 0;

  foreach my $line (@lines)
  {
    # Remove newline characters
    chomp($line);

    if ($line =~ /\/\*/ and $line =~ /\*\//)
    {
      print "Found one-line comment: ", $line, "\n";
      $lines[$lineNum] = "**DELETE_ME**";    
      $wasChanged = 1;
      last; # like break in C++
    }

    if ($line =~ /\/\*/)
    {
      print "Found comment block here: ", $line, "\n";
      $foundCommentBlock = 1;
    }

    if ($line =~ /\*\//)
    {
      print "Found end of comment block: ", $line, "\n";
      $foundCommentBlock = 0;
      last; # like break in C++
    }

    if ($foundCommentBlock == 1)
    {
      $lines[$lineNum] = "**DELETE_ME**";    
      $wasChanged = 1;
    }
    
    $lineNum++;
  }

  if (!$wasChanged)
  {
    print "No existing comment block found!?!\n";
  }

  if ($lines[0] eq "**DELETE_ME**")
  {
    $lines[0] = $COMMENT_BLOCK;
  }
  else
  {
    $lines[0] = $COMMENT_BLOCK . $lines[0];
  }



  # Print new verison of file 
  # -------------------------
  #
  print "\n\nNEW FILE:\n";
  foreach my $line (@lines)
  {
    if ($line ne "**DELETE_ME**")
    {
      print "$line\n";
    }
  }
  print "\n\n";

  if ($ARGV[1] eq "nowrite")
  {
    return;
  }

  print "WRITING OUTPUT TO FILE $fileToModify\n";

  # Print lines to file
  open(MODIFY_THIS, ">$fileToModify");
  # Can't use $line again
  foreach my $line2 (@lines)
  {
    if ($line2 ne "**DELETE_ME**")
    {
      print MODIFY_THIS "$line2" . $/;
    }
  }
  close (MODIFY_THIS);

}

sub MaybeAddHeaders($)
{
  if (/\.cpp$/)
  {
    print "FOUND FILE: $_\n";
    AddHeaders($_);
  }
}

# Main function starts here
# Recursively search directories for files 
find (\&MaybeAddHeaders, $ARGV[0]);


