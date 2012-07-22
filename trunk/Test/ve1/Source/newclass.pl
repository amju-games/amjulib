#!/usr/bin/perl -w

# Create boilerplate code


# Main function starts here
if ($#ARGV != 0)  # !?! size - 1
{
  print "Usage: newclass <classname> e.g. newclass Foo creates Foo.h, Foo.cpp\n";
  exit(0);
}

my $dotH =  $ARGV[0] . ".h";
my $dotCpp =  $ARGV[0] . ".cpp";
my $upper = uc($ARGV[0]);
my $guard =  $upper . "_H_INCLUDED";
my $class =  $ARGV[0];
my $scope = $class . "::";

open (DOT_H, ">$dotH");

print DOT_H "#ifndef $guard\
#define $guard\n\
namespace Amju \
{\
class $class \
{\
public:\
  $class();\
\
};\
} // namespace\
#endif\n";

close (DOT_H);

open (DOT_CPP, ">$dotCpp");

print DOT_CPP "#include \"$dotH\"\
\
namespace Amju\
{\n" . $scope . $class . "()\
{\
}\
} // namespace\n";

close (DOT_CPP);

