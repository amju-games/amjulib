// * Amjulib * Copyright (c) Juliet Colman 2024

#pragma once

#include <AmjuAssert.h>
#include <Colour.h>
#include <LoadVec2.h>
#include "GuiProperty.h"

namespace Amju
{
// Convenience function: get value of the named Vec2f property in the given map
Vec2f GetVec2(const GuiPropertyMap& map, const std::string& name);

// Convenience function: get value of the named Colour property in the given map
Colour GetColour(const GuiPropertyMap& map, const std::string& name);

// Convenience function: get value of the named enum property in the given map
template<class T>
T GetEnum(const GuiPropertyMap& map, const std::string& name)
{
  return static_cast<T>(Get<int>(map, name));
}

// Convenience function: make Vec2f property to store in a GuiPropertyMap
std::string MakeProperty(const Vec2f& v);

// Convenience function: make Colour to store in a GuiPropertyMap
std::string MakeProperty(const Colour& colour);

// Convenience function: make enum property to store in a GuiPropertyMap
template<class T>
std::string MakeEnumProperty(const T& t)
{
  return MakeProperty(static_cast<int>(t));
}
}
