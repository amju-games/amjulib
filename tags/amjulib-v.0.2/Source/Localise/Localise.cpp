/*
Amju Games source code (c) Copyright Jason Colman 2000-2006
*/

#include <AmjuFirst.h>
#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <iostream>
#include "Localise.h"
#include "StringUtils.h"
#include "File.h"
#include <AmjuFinal.h>

//#define LOCALISE_DEBUG

namespace Amju
{
Localise::StringTable Localise::s_strings;

bool Localise::LoadStringTable(const std::string& filename)
{
  AMJU_CALL_STACK;

  // Strings file consists of lines of format
  // <id>=<string>
  // First line is number of strings

  File f;
  if (!f.OpenRead(filename))
  {
    f.ReportError("Localise: Failed to open string table");
    return false;
  }
  int numStrs = 0;
  if (!f.GetInteger(&numStrs))
  {
    f.ReportError("Localise: Expected number of strings");
    return false;
  }
  for (int i = 0; i < numStrs; i++)
  {
    // Parse line to get ID and string
    std::string line;

    if (!f.GetDataLine(&line))
    {
      f.ReportError("Localise: Expected string " + ToString(i + 1) +  " of " + 
        ToString(numStrs));
    }
  
    std::string::size_type eq = line.find('=');
    std::string strCode = line.substr(0, eq);
    std::string str = line.substr(eq + 1);
    int code = atoi(strCode.c_str());

#ifdef LOCALISE_DEBUG
std::cout << "Localise: ID: " << code << ": \"" << str.c_str() << "\"\n";
#endif

    s_strings[code] = str;
  }
  return true;
}

void Lookup(std::string* pResult)
{
  AMJU_CALL_STACK;

  *pResult = Lookup(*pResult);
}

std::string Lookup(const std::string& s)
{
  static const char ID_CHAR = '$'; 

  std::string ret;

  // Localisation
  // If a string starts @@@ it should be converted to $$$<id>
  //  by a script.

  // If a string starts $$$<id>, the ID is to be looked up in the string
  //  table for the current language.

  if (s.size() >= 4 &&
      s[0] == ID_CHAR &&
      s[1] == ID_CHAR &&
      s[2] == ID_CHAR)
  {
    int id = atoi(&(s.c_str()[3]));
    return Localise::GetString(id);
  }
  return s;
}

std::string Localise::GetString(int id)
{
  StringTable::iterator it = s_strings.find(id);
  if (it == s_strings.end())
  {
    Assert(0);
    return "Localise error: string ID " + ToString(id) + " not found";
  }
  return it->second;
}
}


