#!/usr/bin/perl -w

use strict;

  my $fileToModify = "model.mtl";

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

    print MODIFY_THIS "$line2";
    if ($line2 =~ /texture/)
    {
        # Texture line - copy it and adjust for normal map name
        $line2 =~ s/\.png/_normal\.png/;
        $line2 =~ s/map_Kd/map_1/;
        print MODIFY_THIS "$line2";

        # Add shader line 
        print MODIFY_THIS "shader fire_temple/normal-mapped\n";
    }
  }
  close (MODIFY_THIS);

