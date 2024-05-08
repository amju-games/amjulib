// *Amjulib* Copyright (c) Juliet Colman 2024

#pragma once

#include <unordered_map>
#include <RCPtr.h>
#include <Vec2.h>

namespace Amju
{
class IGuiProperty : public RefCounted
{
public:
  virtual ~IGuiProperty() = default;
};

template <typename T>
class GuiProperty : public IGuiProperty
{
public:
  GuiProperty(const T& t) : m_t(t) {}
  const T& Get() const { return m_t; }
  void Set(const T& t) { m_t = t; }

protected:
  T m_t;
};

using PGuiProperty = RCPtr<IGuiProperty>;
using GuiPropertyMap = std::unordered_map<std::string, PGuiProperty>;

// Convenience function: get value of the named property in the given map
template<typename T>
T Get(const GuiPropertyMap& map, const std::string& name)
{
  return dynamic_cast<GuiProperty<T>*>(map.at(name).GetPtr())->Get();
}

// Convenience function: make property to store in a GuiPropertyMap
template<typename T>
PGuiProperty MakeProperty(const T& t)
{
  return new GuiProperty<T>(t);
}

// Convenience function: combine two property maps
GuiPropertyMap Combine(const GuiPropertyMap& map1, const GuiPropertyMap& map2);
}



