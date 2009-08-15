#!/usr/bin/perl -w

#
# Amju Games copyright (c) 2008
# Add callstack macro to functions in C++ source code  
#
# Arguments:
#  - Directory in which to start looking for source files:
#    we then recuse into subdirs.
#  - Optionally, "nowrite" - don't write modified file
#
# Usually run like this:
#   ./AddCallstacks.pl ../../Code [nowrite]
#

use File::Find;
use strict;

sub IsFunctionSig($)
{
  my $line = shift;

#  chomp $line;

#print "IsFunctionSig: $line...?\n";

  # <return type><whitespace><function name><bracket><params><close bracket>
  # A bit tricky as return type could include 'const', 'static', '*',  '&'
  # NB no return type for ctor or dtor!
  # Function name could include weird characters for overloaded operators!

  # \w  alpha num or _
  # \s  whitespace

  if ($line =~ /^#/)
  {
    return 0;
  }

  if ($line =~ /\s*\/\//)
  {
    print "$line is a comment.\n";
    return 0;
  }
 
  $line =~ s/const//;
  $line =~ s/static//;

  if ($line =~ /^\s*\w+\&*\**\s+\w+::\w+\(/ )
  {
    #print "$line could be a c++ function sig.\n";
    return 1;
  }
  
  if ($line =~ /^\s*\w+\&*\**\s+\w+::\w+::\w+\(/ )
  {
    #print "$line could be a nested c++ function sig.\n";
    return 1;
  }
  
  if ($line =~ /^\s*\w+::\w+\(/ )
  {
    #print "$line could be a c++ ctor sig.\n";
    return 1;
  }
  
  if ($line =~ /~.*\(/ )
  {
    #print "$line could be a c++ dtor sig.\n";
    return 1;
  }
  
  if ($line =~ /^\s*\w+::\w+::\w+\(/ )
  {
    #print "$line could be a nested c++ ctor sig.\n";
    return 1;
  }
  
  if ($line =~ /operator/ )
  {
    print "$line looks like a c++ overloaded operator function sig.\n";
    return 1;
  }

  if ($line =~ /^\s*for\s*\(/ or
      $line =~ /^\s*if\s*\(/ or
      $line =~ /^\s*while\s*\(/ or
      $line =~ /^\s*extern/ or
      $line =~ /^\s*namespace/ or
      $line =~ /^\s*switch\s*\(/ 
  )
  {
    #print "$line is not a function sig.\n";
    return 0;
  }
 
  # Check for C-style function with no :: 
  if ($line =~ /^\s*\w+\&*\**\s+\w+\(/ )
  {
    #print "$line could be a c function sig.\n";
    return 1;
  }

  return 0;
}

sub AddCallstack($)
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

  # Check each line of the file for function definitions
 
  my $lineNum = 0;
  my $wasChanged = 0;
  my $lookForCurly = 0; # if 1, we have found a function sig and are now looking for the opening curly bracket
  my $foundCurly = 0; # if 1, insert new code - do we need this ?

  foreach my $line (@lines)
  {
    chomp($line);

    # Remove old call stack lines, so we can re-run this script without
    #  creating duplicates etc
    if ($line =~ /AMJU_CALL_STACK/)
    {
      $lines[$lineNum] = "**DELETE_ME**";
      # Check for blank line also added by me
      if ($lines[$lineNum + 1] =~ /^\s*$/ )
      {
        $lines[$lineNum + 1] = "**DELETE_ME**"; # blank line was also added
      }
    }

    # Search for function signature
    if (!$lookForCurly && IsFunctionSig($line))
    {
      print "Found function ? $line\n";
      $lookForCurly = 1;
    }

    # Find the first curly bracket
    if ($lookForCurly)
    {
      # Line has a curly bracket ?
      if ($line =~ /\{/)
      {
        # Check for closing curly bracket, i.e. empty function {}
        if ($line =~ /\}\s*$/)
        {
          print " -- empty curly brackets.\n";
          $lookForCurly = 0;
        }
        else
        {
          print " -- found opening curly bracket, so this IS a function def\n";
          $foundCurly = 1;
        }
      }
      elsif ($line =~ /\;\s*$/)
      {
        # Found semicolon before opening curly bracket, so this was a 
        # function declaration only.
        print " -- no, just declaration.\n";
        $lookForCurly = 0;
      }
      elsif ($line =~ /^#/)
      {
        # Found a #include etc - probably not a function def.
        $lookForCurly = 0;
      }
    }

    if ($foundCurly)
    {
      $lookForCurly = 0;
      $foundCurly = 0;
      my $newline = $line . "\n  AMJU_CALL_STACK;\n";
      $lines[$lineNum] = $newline;
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

sub MaybeAddCallstack($)
{
  # Only C++ files!
  if (/\.cpp$/)
  {
    print "FOUND FILE: $_\n";
    AddCallstack($_);
  }
}

# Main function starts here
# Recursively search directories for files 
find (\&MaybeAddCallstack, $ARGV[0]);


