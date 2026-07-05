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

# Parse Arguments
my $stringTable = $ARGV[0];
my $searchPath  = $ARGV[1];
my $mode        = $ARGV[2] || ''; 


# Read string table
# -----------------
#
open(STRINGTABLE, $stringTable) or die "No string table specified or can't open it.";

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
find (\&MaybeLocalise, $searchPath);

sub MaybeLocalise($)
{
  if (/\.cpp/)
  {
    print "LOCALISING FILE: $_\n";
    LocaliseCppFile($_);
  }

  elsif ((/\.csv/) or (/\.txt/))
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

# Localise a txt or csv file
# --------------------------
#
# For each line of file, look for all occurrences of @@@<string>
# Add each string found to the string table
# Replace string with $$$<id>
sub LocaliseFile($)
{
    my $fileToLocalise = shift;
    
    unless (open(IN, '<', $fileToLocalise)) {
        print "Error: Could not open $fileToLocalise\n";
        return;
    }
    my @lines = <IN>;
    close(IN);

    my $wasChanged = 0;

    foreach my $line (@lines) {
        # Match @@@ followed by 1 or more characters that are NOT tabs or newlines
        # The /g handles multiple cells on the same line
        if ($line =~ s/@@@([^\t\n\r]+)/
            my $str = $1;
            my $id = AddToStringTable($str);
            print "  -> Found string: '$str' (Assigned ID: $id)\n";
            "\$\$\$$id"; # Replacement string
        /ge) {
            $wasChanged = 1;
        }
    }

    if (!$wasChanged) {
        print "No @@@ strings found in $fileToLocalise\n";
        return;
    }

    # Output logic
    if ($mode eq "nowrite") {
        print "\n--- PREVIEW: $fileToLocalise ---\n";
        print @lines; 
        print "\n--- END PREVIEW ---\n";
    }
    else {
        print "WRITING: $fileToLocalise\n";
        open(OUT, '>', $fileToLocalise) or die "Cannot write to $fileToLocalise: $!";
        print OUT @lines;
        close(OUT);
    }
}

# Localise a cpp file
# -------------------
#
sub LocaliseCppFile {
    my $fileToLocalise = shift;
       
    unless (open(IN, '<', $fileToLocalise)) {
        print "Error: Could not open $fileToLocalise\n";
        return;
    }   
    my @lines = <IN>;
    close(IN);

    my $wasChanged = 0;

    foreach my $line (@lines) {
        if ($line =~ s/\"@@@([^\"]+)\"/'"$$$' . AddToStringTable($1) . '"'/ge) {
            $wasChanged = 1;
        }   
    }   

    return unless $wasChanged;

    print "Found and processed strings in: $fileToLocalise\n";

    if ($mode eq "nowrite") {
        print "--- PREVIEW FOR $fileToLocalise ---\n";
        print @lines; 
        print "--- END PREVIEW ---\n";
    }   
    else {
        open(OUT, '>', $fileToLocalise) or die "Cannot write to $fileToLocalise: $!";
        print OUT @lines;
        close(OUT);
    }       
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
if ($mode eq "nowrite")
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

