#!/usr/bin/perl -w

#
# Amju Games copyright (c) 2006
# Localisation script
#
# Reads source files, converts strings beginning "@@@" to a localised string
#  of form "$$$" followed by an integer ID.
# The ID corresponds to a string in a String Table file. Each line of the
#  String Table is of the form <id>=<string>
#
# Arguments:
#  - "String table" file name
#  - Directory in which to start looking for localisable files:
#    we then recuse into subdirs.
#  - Optionally, "nowrite" - don't write string table or modified file
#
# Usually run like this:
#   ./localise ../../Data/english.txt ../../Code [nowrite]
#   ./localise ../../Data/english.txt ../../Data [nowrite]
#

use File::Find;
use strict;

# Read string table
# -----------------
#
my $stringTable = $ARGV[0];
open(STRINGTABLE, $stringTable) or die "No string table specified";

# Hash of IDs to strings
my %stringHash;

# Read in file. Format for each line is <id>=<string>
my $highestId = 0;
while (<STRINGTABLE>)
{
  chomp;
 
  if ($_ =~ /^\/\// or  $_ =~ /^\d+$/) # comment or number only
  {
    print "Ignoring line: $_ \n";
    next; # like 'continue' in C
  }

  my ($id, $string) = split /=/;
  if ($id ne "")
  {
    $stringHash{$id} = $string;
    if ($id > $highestId)
    {
      $highestId = $id;
    }
  }
}
close(STRINGTABLE);			

# This ID is used for any new strings we find
$highestId++;

print "Number of strings: " . keys(%stringHash) . "\n";
# Debug: print the Hash
while ( my ($key, $value) = each(%stringHash) ) 
{
  print "$key => $value\n";
}


# Search directories for files to localise
find (\&MaybeLocalise, $ARGV[1]);

sub MaybeLocalise($)
{
  if ((/\.cpp/) or (/\.amju2/) or (/\.txt/))
  {
    print "LOCALISING FILE: $_\n";
    LocaliseFile($_);
  }
}

# * AddToStringTable *
# Add string only if it does not already exist in the string table.
# Return the ID of the string.
sub AddToStringTable($)
{
  my $strToAdd = shift;
  print "Adding string $strToAdd to table.. ";

  my @keys = sort { $a <=> $b } keys %stringHash;
  foreach my $key (@keys)
  {
    my $value = $stringHash{$key};
    if ($value eq $strToAdd)
    {
      print "already exists! ID: $key\n";
      return $key;
    }
  }
  
  my $id = $highestId;
  $stringHash{$highestId} = $strToAdd;
  $highestId++;

  print "adding, ID: $id\n";
  return $id;
}

sub LocaliseFile($)
{
  # Read file to localise
  # ---------------------
  #
  # For each line of file, look for all occurrences of @@@<string>
  # Add each string found to the string table
  # Replace string with $$$<id>

  my $fileToLocalise = shift; 
  if (!open(LOCALISE_THIS, $fileToLocalise))
  {
    print "Can't open specified file to localise: $fileToLocalise\n";
    return;
  }

  my @linesToLocalise = <LOCALISE_THIS>;
  close (LOCALISE_THIS);

  # Check each line of the file for @@@ strings.
 
  my $lineNum = 0;
  my $wasChanged = 0;
  foreach my $line (@linesToLocalise)
  {
    chomp($line);
    #print "Considering line: '$line'\n";

    # Look for strings to localise
    # NB Question mark: non-greedy match, in case there is more than one
    # string on a line.
    # TODO This won't handle escaped quotes in strings. 

    # Text files where entire line is a string
    if ($line =~ /^@@@(.*)$/)
    {
      # Add the new string to the string table.
      my $newString = $1; # first group, i.e. the (.*) in match above
      print "New string: $newString\n";

      ####$stringHash{$highestId} = $newString;
      my $id = AddToStringTable($newString);

      # Allocate new ID
      my $localised = $line;
      $localised =~ s/@@@.*/\/\/ $newString\n\$\$\$$id/;
      ###$highestId++;
      print "Localised: $localised\n";

      $linesToLocalise[$lineNum] = $localised;
      $wasChanged = 1;
    }

    # Dictionary-style text files, where a string following = should be localised.
    # There could be more than one such string on a line, e.g. a =@@@b =@@@c
    while ($line =~ /^(.*?)=@@@(.*?)=(.*)$/)
    {
      # Add the new string to the string table.
      my $before = $1;
      my $newString = $2; # SECOND group, i.e. the SECOND (.*?) in match above
      my $after = $3;
      print "Found substr to localise: $newString\n";

      ###$stringHash{$highestId} = $newString;
      my $id = AddToStringTable($newString);

      # Replace second group with localise code
      my $localised = "$before=\$\$\$$id=$after"; 
      ###$highestId++;
      print "Localised: $localised\n";
      $line = $localised;

      $linesToLocalise[$lineNum] = $localised;
      $wasChanged = 1;
    }

    # Handle final =@@@  in a dictionary line
    if ($line =~ /(.*)=@@@(.*?)$/)
    {
      my $before = $1;
      my $newString = $2; # second group, i.e. the (.*?) in match above

      ###$stringHash{$highestId} = $newString;
      my $id = AddToStringTable($newString);

      # Allocate new ID
      my $localised = "$before=\$\$\$$id";
      ###$highestId++;
      print "Localised: $localised\n";

      $linesToLocalise[$lineNum] = $localised;
      $wasChanged = 1;
    }


    # C code, where the line may contain one or more strings in quotes
    # TODO How come this while loop terminates if it doesn't change $line?????
    while ($line =~ /\"@@@(.*?)\"/)
    {
      # Add the new string to the string table.
      my $newString = $1; # first group, i.e. the (.*) in match above
      print "New string: $newString\n";

      ###$stringHash{$highestId} = $newString;
      my $id = AddToStringTable($newString);

      # Allocate new ID
      my $localised = $line;
      $localised =~ s/\"@@@.*?\"/\"\$\$\$$id\" \/\* $newString \*\//;
      ###$highestId++;
      print "Localised: $localised\n";

      $linesToLocalise[$lineNum] = $localised;
      $wasChanged = 1;
    }

    $lineNum++;
  }

  if (!$wasChanged)
  {
    print "No change to this file\n";
    return;
  }


  # Print new verison of file 
  # -------------------------
  #
  print "\n\nNEW FILE:\n";
  foreach my $line (@linesToLocalise)
  {
    print "$line\n";
  }
  print "\n\n";

  if ($ARGV[2] eq "nowrite")
  {
    return;
  }

  print "WRITING OUTPUT TO FILE $fileToLocalise\n";

  # Print localised lines to file
  open(LOCALISE_THIS, ">$fileToLocalise");
  foreach my $line (@linesToLocalise)
  {
    print LOCALISE_THIS "$line\n";
  }
  close (LOCALISE_THIS);

}

# Print string table for checking
# -------------------------------
#
print "NEW STRING TABLE:\n";
print "// version\n1\n// number of strings\n" . keys(%stringHash) . "\n";
my @keys = sort { $a <=> $b } keys %stringHash;
foreach my $key (@keys)
{
  my $value = $stringHash{$key};
  print "$key=$value\n";
}

#
# Quit now if "nowrite" specified
#
if ($ARGV[2] eq "nowrite")
{
  exit(0);
}


# Write the new string table
# --------------------------
#
open(STRINGTABLE, ">$stringTable");
print STRINGTABLE 
  "// version\n1\n// number of strings\n" . keys(%stringHash) . "\n";
###my @keys = sort keys %stringHash;
foreach my $key (@keys)
{
  my $value = $stringHash{$key};
  print STRINGTABLE "$key=$value\n";
}
close(STRINGTABLE);			

