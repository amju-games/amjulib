#!/usr/bin/perl -w

# Create boilerplate code for HTTP requests


# Main function starts here
if ($#ARGV != 0)  # !?! size - 1
{
  print "Usage: newreq <name> e.g. SetPos creates ReqSetPos.h, cpp\n";
  exit(0);
}

my $dotH = "Req" . $ARGV[0] . ".h";
my $dotCpp = "Req" . $ARGV[0] . ".cpp";
my $upper = uc($ARGV[0]);
my $guard = "REQ_" . $upper . "_H_INCLUDED";
my $class = "Req" . $ARGV[0];
my $scope = $class . "::";

open (DOT_H, ">$dotH");

print DOT_H "#ifndef $guard\
#define $guard\n\
#include \"Ve1Req.h\"\n\
namespace Amju \
{\
class $class : public Ve1Req\
{\
public:\
  $class(const std::string& url);\
\
  virtual void HandleResult();\
};\
} // namespace\
#endif\n";

close (DOT_H);

open (DOT_CPP, ">$dotCpp");

print DOT_CPP "#include \"$dotH\"\
\
namespace Amju\
{\n" . $scope . $class . "(const std::string\& url) : Ve1Req(url, \"" . lc($ARGV[0]) . "\")\
{\
}\
\
void " . $scope . "HandleResult()\
{\
\
}\
\
} // namespace\n";

close (DOT_CPP);

