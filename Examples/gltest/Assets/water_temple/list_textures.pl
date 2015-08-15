#!/usr/bin/perl -w

# ----------------------------------------------------------------------
# Find textures which are used in .mtl file, and textures in dir which
#  are not used.
# ----------------------------------------------------------------------

use File::Compare;
use List::Util qw(first);

use strict;

sub test_file_compare()
{
  my @files = <*.png>;

  my $filesCompared = 0;

  foreach my $file (@files)
  {
    print $file . ", comparing with " . $filesCompared . " files.\n";

    for (my $i = 0; $i < $filesCompared; $i++)
    {
      my $compFile = $files[$i];
      #print " - Compare " . $file . " and " . $compFile . "\n";

      if (compare($file, $compFile) == 0)
      {
        print " - SAME: " . $file . " and " . $compFile . "\n";
      }
    }

    $filesCompared++;
  }
}


  my @files = <*.png>;
  my @filesToKeep;
  my @filesToDelete;

  my $numMaterialsFound = 0;
  my $fileToModify = "material.mtl.new.new";

  if (!open(MODIFY_THIS, $fileToModify))
  {
    print "Can't open specified file to modify: $fileToModify\n";
    return;
  }

  my @lines = <MODIFY_THIS>;
  close (MODIFY_THIS);


  print "Listing used/unused textures in $fileToModify\n";

  # Print lines to file
  # Can't use $line again
  foreach my $line2 (@lines)
  {
    # Remove newline characters
#    chomp($line2);

    $line2 =~ s/map_Ka/map_Kd/;

    # If this line specifies a texture, look for first texture file which is the same, so all materials
    #  which have an identical-looking texture actually use the same file.
    # Then all textures in the .mtl file should be different.
    if ($line2 =~ /map_Kd/)
    {
      my $tex = $line2;
      $tex =~ s/map_Kd //;
      $tex =~ s/\r//;
      $tex =~ s/\n//;
      $numMaterialsFound++;
      print $numMaterialsFound . ". Found map_Kd, texture is: '" . $tex . "'...\n";

      # Check if element is already in array:
      #  http://www.perlmonks.org/?node_id=681629 

      if (not first { $_ eq $tex } @filesToKeep)
      { 
        print "  (That's a new file)\n";
        push(@filesToKeep, $tex); # only if not already in @filesToKeep
      }
      else
      {
        print "  (Already seen this file)\n";
      }

      # Remove $tex from @files array is astonishingly, shit headedly complex.
      # @array = grep { $_ != $element_omitted } @array;
      #  from http://stackoverflow.com/questions/174292/what-is-the-best-way-to-delete-a-value-from-an-array-in-perl
      ## @filesToDelete = grep { $_ != $tex } @filesToDelete; 
    }
  }

  print "Found " . $numMaterialsFound . " materials.\n";
  my $numToKeep = @filesToKeep;
  print "Files to keep ($numToKeep): ";
  #print "@filesToKeep\n";
 
  # Look at each file. If not in @filesToKeep, we should delete it. 
  my $numToDelete = 0; 
  #print "Files to delete:\n";
  my $numFiles = @files;
  print "Num files: $numFiles \n";
  foreach my $f (@files)
  {
    if (first { $_ eq $f} @filesToKeep)
    {
#      print "We are keeping file $f\n";
    }
    else
    {
      print "rm file $f\n";
      $numToDelete++;
    }

  }
  print "\nNum files to delete: $numToDelete \n";
