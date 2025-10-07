/*
Amju Games source code (c) Copyright Juliet Colman 2000-2006
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
//#define LOCALISE_DEBUG_VERBOSE

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

std::string LookupMulti(
  const std::string& cs, 
  std::function<std::string(int)> getStringFunc)
{
  std::string s(cs);

  size_t f = s.find("$$$");
  while (f != std::string::npos)
  {
#ifdef LOCALISE_DEBUG_VERBOSE
std::cout << "Found $$$ at pos " << f << "\n";
#endif

    int end = f + 3;
    // Get the position of the character following the int.
    while (isdigit(s[end]))
    {
      end++;
    }
#ifdef LOCALISE_DEBUG_VERBOSE
std::cout << "Found non-digit at pos " << end << "\n";
#endif

    // Find int following f, but don't include any chars after the int.
    int i = ToInt(s.substr(f + 3, end - f));

#ifdef LOCALISE_DEBUG_VERBOSE
std::cout << "Found id: " << i << "\n";
#endif
    
    // Replace "$$$<i>" with looked up string
    std::string tail = s.substr(end);

#ifdef LOCALISE_DEBUG_VERBOSE
std::cout << "Tail: '" << tail << "'\n";
#endif

    s = s.substr(0, f) + getStringFunc(i) + tail;

#ifdef LOCALISE_DEBUG_VERBOSE
std::cout << "New s: " << s << "\n";
#endif

    f = s.find("$$$", f); // Search again for $$$, from f onwards

#ifdef LOCALISE_DEBUG_VERBOSE
std::cout << "New f: " << f << "\n";
#endif
  }

  return s;
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


