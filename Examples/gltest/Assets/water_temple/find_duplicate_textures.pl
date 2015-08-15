#!/usr/bin/perl -w

# ----------------------------------------------------------------------
# Find duplicate textures
# ----------------------------------------------------------------------

use File::Compare;

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
  my $duplicatesFound = 0;
  my $fileToModify = "material.mtl.new";

  if (!open(MODIFY_THIS, $fileToModify))
  {
    print "Can't open specified file to modify: $fileToModify\n";
    return;
  }

  my @lines = <MODIFY_THIS>;
  close (MODIFY_THIS);


  print "WRITING OUTPUT TO FILE $fileToModify\n";

  # Print lines to file
  open(MODIFY_THIS, ">$fileToModify.new");
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
      print $numMaterialsFound . ". Found map_Kd, texture is: '" . $tex . "'...\n";
   
      my $foundCopy = 0; 
      my $compFile;
      for (my $i = 0; $i < @files && $foundCopy == 0; $i++)
      {
        $compFile = $files[$i];
        #print " - Compare " . $tex . " and " . $compFile . "\n";

        if (compare($tex, $compFile) == 0)
        {
          if ($tex eq $compFile)
          {
            push(@filesToKeep, $tex);
          }
          else
          {
            print " - SAME: " . $tex . " and " . $compFile . "\n";
            push(@filesToDelete, $tex);
            $duplicatesFound++;
          }
          $foundCopy = 1;
        }
      }
      $numMaterialsFound++;
      if ($foundCopy)
      {
        $line2 = "map_Kd $compFile";
      }
    }
    
    print MODIFY_THIS "$line2";
    if ($line2 =~ /^newmtl/)
    {
      print MODIFY_THIS "Ns 1.0\n"; 
    }
  }
  close (MODIFY_THIS);

  print "Found " . $numMaterialsFound . " materials. Found " . $duplicatesFound . " duplicates!\n";
  my $numToKeep = @filesToKeep;
  print "Files to keep ($numToKeep): @filesToKeep\n";
  
  my $numToDelete = @filesToDelete;
  print "Files to delete ($numToDelete): @filesToDelete\n";


