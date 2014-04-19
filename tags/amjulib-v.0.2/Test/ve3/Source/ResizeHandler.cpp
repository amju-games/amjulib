#include <AmjuFirst.h>
#include <iostream>
#include <ConfigFile.h>
#include "ResizeHandler.h"
#include <AmjuFinal.h>

namespace Amju
{
bool ResizeHandler::OnResizeEvent(const ResizeEvent& re)
{
  static ConfigFile* cf = TheGameConfigFile::Instance();
  cf->SetInt("screen-x", re.x);
  cf->SetInt("screen-y", re.y);

  return true;
}
}

