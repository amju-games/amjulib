#include <iostream>
#include <ConfigFile.h>
#include "ResizeHandler.h"

namespace Amju
{
bool ResizeHandler::OnResizeEvent(const ResizeEvent& re)
{
std::cout << "RESIZE EVENT! Save this shit to game.cfg!!!\n";

  ConfigFile* cf = TheGameConfigFile::Instance();
  cf->SetInt("screen-x", re.x);
  cf->SetInt("screen-y", re.y);

  return true;
}
}

