#!/usr/bin/perl -w

# Create boilerplate code


# Main function starts here
if ($#ARGV != 0)  # !?! size - 1
{
  print "Usage: newstate <statename> e.g. Lighting creates GSLighting.h, cpp\n";
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
#include \"GSBase.h\"\n\
namespace Amju \
{\
class $class : public GSBase\
{\
  $class();\
  friend class Singleton<$class>;\
\
public:\
  virtual void Update() override;\
  virtual void DrawScene() override;\
  virtual void CreateTweakMenu() override;
  virtual void OnActive() override;\
\
  virtual bool OnCursorEvent(const CursorEvent&) override;\
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&) override;\
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
  //m_nextState=...
}\
\
void " . $scope . "Update()\
{\
  GSBase::Update();\
\
}\
\
void " . $scope . "DrawScene()\
{\
\
}\
\
void " . $scope . "CreateTweakMenu()\
{\
  GSBase::CreateTweakMenu();\
}\
\
void " . $scope . "OnActive()\
{\
  GSBase::OnActive();\
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

