#!/usr/bin/perl -w

use strict;

  my $fileToModify = "loc1.mtl";

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
    if ($line2 =~ /^newmtl/)
    {
      print MODIFY_THIS "Ns 1.0\n"; 
    }
  }
  close (MODIFY_THIS);

