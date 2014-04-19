#!/usr/bin/perl -w

# Create boilerplate code for GUI-based class


# Main function starts here
if ($#ARGV != 0)  # !?! size - 1
{
  print "Usage: newgui <statename> e.g. Lighting creates GSLighting.h, cpp\n";
  exit(0);
}

my $dotH = "GS" . $ARGV[0] . ".h";
my $dotCpp = "GS" . $ARGV[0] . ".cpp";
my $upper = uc($ARGV[0]);
my $guard = "GS_" . $upper . "_H_INCLUDED";
my $class = "GS" . $ARGV[0];
my $scope = $class . "::";

open (DOT_H, ">$dotH");

print DOT_H "#ifndef $guard\
#define $guard\n\
#include <Singleton.h>\
#include \"GSGui.h\"\n\
namespace Amju \
{\
class $class : public GSGui\
{\
  $class();\
  friend class Singleton<$class>;\
\
public:\
  virtual void Update();\
  virtual void Draw();\
  virtual void Draw2d();\
  virtual void OnActive();\
\
  virtual bool OnCursorEvent(const CursorEvent&);\
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);\
};\
typedef Singleton<$class> The$class;\
} // namespace\
#endif\n";

close (DOT_H);

open (DOT_CPP, ">$dotCpp");

print DOT_CPP "#include \"$dotH\"\
#include <AmjuGL.h>\
\
namespace Amju\
{\n" . $scope . $class . "()\
{\
}\
\
void " . $scope . "Update()\
{\
  GSGui::Update();\
\
}\
\
void " . $scope . "Draw()\
{\
  GSGui::Draw();\
\
}\
\
void " . $scope . "Draw2d()\
{\
  GSGui::Draw2d();\
}\
\
void " . $scope . "OnActive()\
{\
  GSGui::OnActive();\
}\
\
bool " . $scope . "OnCursorEvent(const CursorEvent& ce)\
{\
  return false;\
}\
\
bool " . $scope . "OnMouseButtonEvent(const MouseButtonEvent& mbe)\
{\
  return false;\
}\
} // namespace\n";

close (DOT_CPP);

