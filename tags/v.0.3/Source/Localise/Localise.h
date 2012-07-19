/*
Amju Games source code (c) Copyright Jason Colman 2000-2006
*/

#ifndef LOCALISE_H_INCLUDED
#define LOCALISE_H_INCLUDED

#include <map>
#include <string>

namespace Amju
{
// Look up a localised string ("$$$<id>") and return the
//  real string.
std::string Lookup(const std::string&);

// Convenience function, lookup string in place
void Lookup(std::string* pResult);

class Localise
{
public:
  static bool LoadStringTable(const std::string& filename);

  static std::string GetString(int id);

private:
  typedef std::map<int, std::string> StringTable;
  static StringTable s_strings;
};
}

#endif

