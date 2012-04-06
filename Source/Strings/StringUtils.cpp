/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: StringUtils.cpp,v $
Revision 1.10  2008/05/08 10:58:06  jay
New memory management code

Revision 1.9  2008/03/21 19:52:38  jay
MSVC fix

Revision 1.8  2008/03/18 09:25:45  jay
Move Filter() function to StringUtils

Revision 1.7  2006/12/10 12:10:32  jay
Trim: check for newlines

Revision 1.6  2006/12/03 21:27:58  jay
Pad one-digit hex number

Revision 1.5  2006/10/11 00:21:34  Administrator
MSVC fix

Revision 1.4  2006/10/09 20:26:52  jay
Add InsertCommas()

Revision 1.3  2006/09/17 23:00:40  Administrator
Big APZ weekend changes

Revision 1.1.10.2  2006/07/27 08:43:52  jay
Add ToHexString()

Revision 1.1.10.1  2005/05/09 05:54:20  jay
Added ToString(float) with parameter for no of decimal digits

Revision 1.1  2004/09/08 15:43:20  jay
Added to repository
  
*/

#include "AmjuFirst.h"

#ifdef WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif // WIN32

#include <algorithm> // remove()
#include <functional> // for MSVC
#include <iostream>
#include <string.h> // strlen()
#include <stdio.h> // sprintf()
#include <sstream>
#include "StringUtils.h"
#include "AmjuAssert.h"
#include "AmjuFinal.h"

namespace Amju
{
struct Remover : public std::unary_function<std::string, bool>
{
  Remover(const std::string s) : m_s(s) {}
  bool operator()(const std::string& s) const
  {
  AMJU_CALL_STACK;

    return s.find(m_s) == std::string::npos;
  }
  std::string m_s;
};

void Filter(
  std::vector<std::string>* pResult,
  const std::string& substr,
  bool include)
{
  AMJU_CALL_STACK;

  if (include)
  {
    pResult->erase(
      std::remove_if(pResult->begin(), pResult->end(), Remover(substr)),
      pResult->end()
    );
  }
  else
  {
    pResult->erase(
      std::remove_if(pResult->begin(), pResult->end(),
        std::unary_negate<Remover>(substr)),
      pResult->end()
    ); 
  }
}


bool StringContains(const std::string& container, const std::string& substr)
{
  AMJU_CALL_STACK;

  size_t f = container.find(substr);
  return (f != std::string::npos);
}

std::string StripPath(const std::string& s, const std::string& slashChar)
{
  std::string ret = s;
  size_t f = ret.find_last_of(slashChar);
  if (f != std::string::npos)
  {
    ret = ret.substr(f + 1);
  }
  return ret;
}

std::string StripPath(const std::string& s)
{
  // Return string, but minus any path info. 
  // To do this, we search from the end of the string for the path separator
  // character. If we find this character, we strip it and anything before it.

  // Strip backslash and forward slash characters.
  return StripPath(StripPath(s, "/"), "\\");
}

std::string GetFilePath(const std::string& filename)
{
  const std::string slash = "/"; 
  return GetFilePath(filename, slash);
}

std::string GetFilePath(const std::string& s, const std::string& slashChar)
{
  size_t f = s.find_last_of(slashChar);
  if (f != std::string::npos)
  {
    return s.substr(0, f);
  }
  return ""; // No slash: only filename, no path. So return empty string for path.
}

void Trim(std::string* ps)
{
  AMJU_CALL_STACK;

  Assert(ps);
  while (!ps->empty() && (ps->at(0) == ' ' || ps->at(0) == '\r' || ps->at(0) == '\n' ) )
  {
    *ps = ps->substr(1);
  }
  while (!ps->empty() && (ps->at(ps->size() - 1) == ' ' || ps->at(ps->size() - 1) == '\r' || ps->at(ps->size() - 1) == '\n') )
  {
    *ps = ps->substr(0, ps->size() - 1);
  }
}

std::string GetFileExt(const std::string& f)
{
  size_t pos = f.rfind(".");

  if (pos != std::string::npos)
  {
    return f.substr(pos + 1);
  }
  return "";
}

std::string GetFileNoExt(const std::string& f)
{
  // Return all up to but not including the final dot.
  size_t pos = f.rfind(".");

  if (pos != std::string::npos)
  {
    return f.substr(0, pos);
  }
  return f;
}

std::string ToLower(const std::string& s)
{
  std::string result;
  for (unsigned int i = 0; i < s.size(); i++)
  {
#ifdef WIN32
    result += tolower(unsigned char(s[i]));
#else
    result += tolower(int(s[i]));
#endif
  }
  return result;
}

std::string Remove(const std::string& s, char c)
{
  std::string r(s);
  r.erase(std::remove(r.begin(), r.end(), c), r.end());
  return r;
}

std::string ToHexString(unsigned int i)
{
  char buf[50];
  sprintf(buf, "%02x", i);
  return buf;
}

unsigned int UIntFromHexString(const std::string& s)
{
  // From http://stackoverflow.com/questions/1070497/c-convert-hex-string-to-signed-integer
  unsigned int x;   
  std::stringstream ss;
  ss << std::hex << s;
  ss >> x;
  return x;
}

std::string ToString(unsigned int i)
{
  char buf[50];
  sprintf(buf, "%u", i);
  return buf;
}

std::string ToString(int i)
{
  char buf[50];
  sprintf(buf, "%d", i);
  return buf;
}

std::string ToString(float f)
{
  char buf[50];
  sprintf(buf, "%f", f);
  return buf;
}

std::string ToString(float f, int digits)
{
  char buf[50];
  std::string s = "%." + ToString(digits) + "f";
  sprintf(buf, s.c_str(), f);
  return buf;
}

float ToFloat(const std::string& s)
{
  return (float)atof(s.c_str());
}

int ToInt(const std::string& s)
{
  return atoi(s.c_str());
}

std::string Replace(
  const std::string& s, 
  const std::string& replaceThis, 
  const std::string& replaceWithThis)
{
  std::string r(s);
  size_t j;
  int length = replaceThis.length();

  while ((j = r.find(replaceThis)) != std::string::npos)
  {
    r.replace(j, length, replaceWithThis);
  }
  return r;
}

Strings Split(const std::string& cs, char c)
{
  Strings r;
  std::string s(cs);
  while (true) 
  {
    size_t i = s.find(c);
    if (i == std::string::npos)
    {
      // No special character found, so push back the entire string and finish.
      r.push_back(s);
#ifdef SPLIT_DEBUG
      std::cout << "Split: final string: " << s.c_str() << "\n";
#endif
      break; 
    }    
    else
    {
      // Found the special character. 
      // Push back whatever is before the character, then work on the remainder
      // of the string.
#ifdef SPLIT_DEBUG
      std::cout << "Split: string: " << s.substr(0, i).c_str() << " rem: " << s.substr(i + 1).c_str() << "\n";
#endif 
      r.push_back(s.substr(0, i));
      s = s.substr(i + 1);
    }
  }
  return r;
}

std::string InsertCommas(const char* numStr)
{
   std::string resultStr;

   int s = strlen(numStr);
   int c = s % 3;
   if (c == 0)
   {
       c = 3;
   }
   for (int i = 0; i < s; i++)
   {
#ifndef MSVC
       resultStr.push_back(numStr[i]);
#else
       resultStr += std::string(1, numStr[i]);
#endif
       --c;
       if (c == 0 && i < (s - 2))
       {
           c = 3;
#ifndef MSVC
           resultStr.push_back(',');
#else
           resultStr += std::string(1, ',');
#endif
       }
   }
   return resultStr; 
}

}

