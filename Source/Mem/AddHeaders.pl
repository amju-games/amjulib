#!/usr/bin/perl -w

#
# Amju Games copyright (c) 2008
# Add header files to C++ source code  
#
# Arguments:
#  - Directory in which to start looking for source files:
#    we then recurse into subdirs.
#  - Optionally, "nowrite" - don't write modified file
#
# Usually run like this:
#   ./AddHeaders.pl ../../Code [nowrite]
#

use File::Find;
use strict;

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

  # Check each line of the file for includes.
  # When we find the first include, prepend the First include.
  # Keep track of the current include in case it turns out to be the final one.
  # Then we add the final include to it.
  # If no includes yet, just do nothing. 
  # First include:  #include "AmjuFirst.h"
  # Last include:   #include "AmjuFinal.h"
 
  my $lineNum = 0;
  my $wasChanged = 0;
  my $foundFirst = 0;
  my $lastIncludeLine = 0;

  foreach my $line (@lines)
  {
    chomp($line);

    # Remove old lines we added, so we can re-run this script without
    #  creating duplicates 
    if ($line =~ /AmjuFirst\.h/ or $line =~ /AmjuFinal\.h/)
    {
      $lines[$lineNum] = "**DELETE_ME**";
    }

    # If first line is #ifdef etc, put #include "AmjuFirst.h" before it
    if ($line =~ /^#/)
    {
      if (!$foundFirst)
      {
        $foundFirst = 1;
        my $newline = "#include \"AmjuFirst.h\"\n" . $line;
        $lines[$lineNum] = $newline;
        $wasChanged = 1;
      }
    }

    # Look for final #include
    if ($line =~ /^#include/)
    {
      $lastIncludeLine = $lineNum;
    }

    $lineNum++;
  }

  if (!$wasChanged)
  {
    print "No change to this file\n";
    return;
  }

  # Add final include
  my $line = $lines[$lastIncludeLine];

#print "Append final to: $line\n";

  if ($line eq "**DELETE_ME**")
  {
    $lines[$lastIncludeLine] = "#include \"AmjuFinal.h\"";
  }
  else
  {
    $lines[$lastIncludeLine] = $line . "\n#include \"AmjuFinal.h\"";
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
      print MODIFY_THIS "$line2\n";
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


