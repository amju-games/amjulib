// * Amjulib * Copyright (c) Juliet Colman 2024

#include "GuiPropertyHelpers.h"

namespace Amju
{
std::string MakeProperty(const Vec2f& v)
{
  return ToString(v);
}

std::string MakeProperty(const Colour& colour)
{
  constexpr bool INCLUDE_ALPHA = true;
  return ToHexString(colour, INCLUDE_ALPHA);
}

Colour GetColour(const GuiPropertyMap& map, const std::string& name)
{
  const std::string& value = map.at(name);
  return FromHexString(value);
}

Vec2f GetVec2(const GuiPropertyMap& map, const std::string& name)
{
  const std::string& value = map.at(name);
  Vec2f v;
  if (!ToVec2(value, &v))
  {
    Assert(false);
  }
  return v;
}

}
