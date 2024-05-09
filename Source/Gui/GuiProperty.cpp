// *Amjulib* Copyright (c) Juliet Colman 2024

#include "GuiProperty.h"

namespace Amju
{
GuiPropertyMap Combine(const GuiPropertyMap& map1, const GuiPropertyMap& map2)
{
  GuiPropertyMap result(map1);
  result.insert(map2.begin(), map2.end());
  return result;
}

std::string MakeProperty(const std::string& s)
{
  return s;
}
}
