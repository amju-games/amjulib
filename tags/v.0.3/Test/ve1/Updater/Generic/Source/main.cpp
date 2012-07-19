// Command-line updater - could be used on Wii.
// Reference for other platforms/techs

#include <iostream>
#include "Updater.h"

using namespace Amju;

void Report(const char* s)
{
  std::cout << s;
}

int main(int argc, char** argv)
{
  std::cout << "My Game... checking for client updates...";

  Updater u(Report);
  u.Work(); // not Start(), no need to create another thread


  return 0;
}

