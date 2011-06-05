#!/usr/bin/perl -w

#
# Amju Games copyright (c) Jason Colman 2009
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

my $COMMENT_BLOCK = "/*\nAmju Games source code (c) Copyright Jason Colman 2000-2011\n*/\n\n";

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

    # Break when we find the first line which is blank or not a comment

    if (($line =~ /\/\*/ and $line =~ /\*\//) or $line =~ /^\/\//)
    {
      print "Found one-line comment: line ", $lineNum+1, "\n";
      $foundCommentBlock = 0;
      $lines[$lineNum] = "**DELETE_ME**";    
      $wasChanged = 1;
      ###last; # like break in C++
    }
    elsif ($line =~ /\/\*/)
    {
      print "Found comment block start, line ", $lineNum+1, "\n";
      $foundCommentBlock = 1;
    }
    elsif ($line =~ /\*\//)
    {
      print "Found end of comment block, line ", $lineNum+1, "\n";
      $foundCommentBlock = 0;
      $lines[$lineNum] = "**DELETE_ME**";    
      $wasChanged = 1;
      #last; # like break in C++ -- i.e. we are done
    }
    elsif ($line eq "")
    {
      print "Found empty line, line ", $lineNum+1, "\n";
      $lines[$lineNum] = "**DELETE_ME**";    
      $wasChanged = 1;
    }
    elsif ($foundCommentBlock == 0)
    {
      # not comment or blank
      print "Found code, line ", $lineNum+1, "\n";
      last; 
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
    # Remove newline characters
    chomp($line);

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
    # Remove newline characters
    chomp($line2);

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


