// * Amjulib * Copyright (c) Juliet Colman 2024

#pragma once

#include <string>
#include <sstream>
#include <unordered_map>

namespace Amju
{
// Property map: maps name of property to its value, stored as a string.
// All properties can be stored as strings.
using GuiPropertyMap = std::unordered_map<std::string, std::string>;

// Convenience function: get value of the named property in the given map
template<typename T>
T Get(const GuiPropertyMap& map, const std::string& name)
{
  T t;
  const std::string& value = map.at(name);
  std::stringstream(value) >> t;
  return t;
}

// Convenience function: make property to store in a GuiPropertyMap
template<typename T>
std::string MakeProperty(const T& t)
{
  return std::to_string(t);
}

// Convenience function: make property to store in a GuiPropertyMap
// Special case for strings
std::string MakeProperty(const std::string& s);

// Convenience function: combine two property maps.
// TODO Why no merge?
GuiPropertyMap Combine(const GuiPropertyMap& map1, const GuiPropertyMap& map2);
}



