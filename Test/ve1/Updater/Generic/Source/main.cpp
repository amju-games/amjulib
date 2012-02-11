// Command-line updater - could be used on Wii.
// Reference for other platforms/techs

#include <iostream>
#include "Updater.h"

using namespace Amju;
 
int main(int argc, char** argv)
{
  std::cout << "My Game... checking for client updates...";

  TheUpdater::Instance()->Start();


  return 0;
}

