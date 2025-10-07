/*
Amju Games source code (c) Copyright Juliet Colman 2000-2018
*/

#ifndef LOCALISE_H_INCLUDED
#define LOCALISE_H_INCLUDED

#include <functional>
#include <map>
#include <string>

namespace Amju
{
class Localise
{
public:
  static bool LoadStringTable(const std::string& filename);

  static std::string GetString(int id);

private:
  typedef std::map<int, std::string> StringTable;
  static StringTable s_strings;
};

// Look up a localised string ("$$$<id>") and return the
//  real string.
std::string Lookup(const std::string&);

// Replace multiple localised strings. E.g. "Hello $$$1 $$$2" will replace
//  two strings, preserving "Hello" and the whitespace.
// Second parameter is the function we use to look up strings, for testing.
std::string LookupMulti(
  const std::string&, 
  std::function<std::string(int)> getStringFunc = Localise::GetString);

// Convenience function, lookup string in place
// TODO Deprecate
void Lookup(std::string* pResult);
}

#endif

