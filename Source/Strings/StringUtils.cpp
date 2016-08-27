/*
Amju Games source code (c) Copyright Jason Colman 2004-2016
*/

#include <AmjuFirst.h>
#ifdef WIN32
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#endif // WIN32

#include <algorithm> // remove()

#ifdef ANDROID_NDK
#include <ctype.h>
#endif

#include <functional> // for MSVC
#include <iostream>
#include <string.h> // strlen()
#include <stdio.h> // sprintf()
#include <sstream>

#include <AmjuAssert.h>
#include "StringUtils.h"

#include <AmjuFinal.h>

namespace Amju
{
// Correctly truncate utf-8 string
// Based on: http://trinitum.org/wp/how-to-truncate-utf8-string/
std::string TruncateUtf8String(const std::string& str, int maxSize)
{
  size_t len = str.length();
  if (len <= maxSize)
  {
    return str;
  }
  
  std::string res = str;
  
  if (res[maxSize] & 128)
  {
    // Is multibyte char: look for first byte and return substring up to but not including this
    //  first byte.
    if (res[maxSize] & 64)
    {
      res = res.substr(0, maxSize);
    }
    else if (res[maxSize - 1] & 64)
    {
      res = res.substr(0, maxSize - 1);
    }
    else if (res[maxSize - 2] & 64)
    {
      res = res.substr(0, maxSize - 2);
    }
    else
    {
      res = res.substr(0, maxSize - 3);
    }
  }
  else
  {
    // Not a mutibyte char: just truncate.
    res = res.substr(0, maxSize);
  }
  
  size_t newlen = res.length();
  Assert(newlen <= maxSize);
  return res;
}

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
  const std::string slash = "/\\"; 
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

std::string CleanPath(const std::string& path)
{
  // replace "/<dir>/../" with "/" (except "/../../" and "/./../")
  std::string s = Replace(path, "\\", "/");
  s = Replace(s, "//", "/");

  //    /  <dir>  /   ..  /
  //    ^    ^    ^   ^   ^
  //    f1  d1   f2   d2  f3

  std::string::iterator f1 = std::find(s.begin(), s.end(), '/');
  while (f1 != s.end())
  {
    std::string::iterator f2 = std::find(f1 + 1, s.end(), '/');
    if (f2 == s.end())
    {
      break;
    }
    std::string::iterator f3 = std::find(f2 + 1, s.end(), '/');
    //if (f3 == s.end())
    //{
      // OK if end of string
      //break;
    //}
    std::string d1(f1 + 1, f2);
    std::string d2(f2 + 1, f3);
    if (d1 != "." && d1 != ".." && d2 == "..")
    {
      s = std::string(s.begin(), f1) + std::string(f3, s.end());
      f1 = std::find(s.begin(), s.end(), '/');
    }
    else
    {
      f1 = f2;
    }
  }

  if (s.empty())
  {
    s = "/";
  }

#ifdef CLEAN_PATH_DEBUG
std::cout << "CleanPath: \"" << path << "\" => \"" << s << "\"\n";
#endif

  return s;
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

std::string ToHexString(int i)
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

std::string Replace(const std::string& str1, const std::string& oldStr, const std::string& newStr)
{
  std::string str(str1);
  size_t pos = 0;
  while ((pos = str.find(oldStr, pos)) != std::string::npos)
  {
    str.replace(pos, oldStr.length(), newStr);
    pos += newStr.length();
  }
  return str;
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
      Trim(&s); // Allow whitespace, get rid of it here
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

