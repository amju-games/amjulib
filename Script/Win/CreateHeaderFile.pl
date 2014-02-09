#!/usr/bin/perl -w

#
# Amju Games copyright (c) 2014
# Create <amju.h>
#
# Arguments:
#  - Directory in which to start looking for source files:
#    we then recurse into subdirs.
#
# Prints to stdout - redirect to file on command line.
#
# Usually run like this:
#   ./CreateHeaderFile.pl ../../Build/amjulib/include > 
#      ../../Build/amjulib/include/amju.h
#

use File::Find;
use strict;


sub MaybeAddHeaders($)
{
  if (/\.h$/ && !/main.h$/)
  {
    print "#include <$_>\n"; 
  }
}

# Main function starts here
# TODO Print comment header

print "#pragma once\n";

# Recursively search directories for files 
find (\&MaybeAddHeaders, $ARGV[0]);


