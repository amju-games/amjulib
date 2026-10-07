// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026

#pragma once

#include <functional>
#include <unordered_map>
#include <string>

namespace Amju
{
// * Function Factory *
// This kind of Factory is useful for mapping strings to 
//  functions, all with the same signature.
template <typename F, typename S = std::string>
class FunctionFactory
{
public:
  // Add Function f for the given name.
  bool Add(const S& name, F f)
  {
    m_map[name] = f;
    return true;
  }

  // Return the Function for the given name. 
  F Create(const S& name)
  {
    auto it = m_map.find(name);
    if (it == m_map.end())
      return nullptr;

    return it->second;
  }

protected:
  using FuncMap = std::unordered_map<S, F>;
  FuncMap m_map;
};
}
