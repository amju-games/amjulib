/*
Amju Games source code (c) Copyright Jason Colman 2004-2009
*/

#ifndef STRING_UTILS_H_INCLUDED
#define STRING_UTILS_H_INCLUDED

#include <algorithm>
#include <string>
#include <vector>

namespace Amju
{
// Helper: strips off any path info from a filename.
std::string StripPath(const std::string& s);
std::string StripPath(const std::string& s, const std::string& slashChar);

// Helper: trim spaces off beginning and end of string.
void Trim(std::string* pStr);

// Get the extension (letters following final dot) of a filename.
std::string GetFileExt(const std::string& filename);

// Get the filename _without_ the final dot and extension.
std::string GetFileNoExt(const std::string& f);

// Get only the path, i.e. remove the final filename from the string.
std::string GetFilePath(const std::string& filename);
std::string GetFilePath(const std::string& filename, const std::string& slashChar);

// Clean path + optional filename: replace "/<dir>/../" with "/"
std::string CleanPath(const std::string& path);

// Convert string to lower case.
std::string ToLower(const std::string& s);

// Remove character c from the string.
std::string Remove(const std::string& s, char c);

// Convert int to string
std::string ToString(int i);
std::string ToString(unsigned int i);

// Convert int to hex string
std::string ToHexString(unsigned int i);
unsigned int UIntFromHexString(const std::string& s);

// Convert float to string
std::string ToString(float f);
// This overload lets you set the number of digits after the decimal point.
std::string ToString(float f, int digits);

// Convert a string to a float
float ToFloat(const std::string& s);

// Convert a string to an int
int ToInt(const std::string& s);


// Returns a string in which replaceThis is replaced with replaceWithThis.
// Known bug: replaceWithThis must NOT contain the string replaceThis.
// This condition causes an infinite loop, until the string is so big you 
// run out of memory.
std::string Replace(
  const std::string& s, 
  const std::string& replaceThis, 
  const std::string& replaceWithThis);

// Returns true if container string contains substr.
bool StringContains(const std::string& container, const std::string& substr);

// Split a string into a vector of substrings. 
// split_char is not included in the substrings.
typedef std::vector<std::string> Strings;
Strings Split(const std::string& text, char split_char);

// Convenience function which filters entries from the given vector of
// strings.
// If include is true, only strings which contain the given
// substring are retained.
// If include is false, strings which contain the given substring
// are filtered out.
void Filter(
  std::vector<std::string>* pResult,
  const std::string& substr,
  bool include = true);


// Insert commas into a number string
// Adds a comma between every third digit, e.g. 12345 => 12,345 etc.
std::string InsertCommas(const char* numStr);


// Word Wrap
// WIDTH_FINDER must support float operator() (const std::string&)
// returning the width of the string.
template <class WIDTH_FINDER>
Strings WordWrap(std::string ss, float maxW, WIDTH_FINDER t)
{
  // Start off by splitting by newlines in s. Then we word wrap the strings.
  ss = Remove(ss, '\r');
  Strings longStrs = Split(ss, '\n');
  Strings strs;

  for (Strings::iterator it = longStrs.begin(); it != longStrs.end(); ++it)
  {
    std::string s = *it;
    Trim(&s);
    if (s.empty())
    {
      strs.push_back("");
      continue;
    }

    std::string rem;
    while (!s.empty())
    {
      if (t(s) <= maxW ||
          std::find(s.begin(), s.end(), ' ') == s.end() ) // no space in s
      {
        strs.push_back(s);
        s = rem;
        if (!s.empty() && s[0] == ' ')
        {
          s = s.substr(1); // remove leading space
        }
        rem = "";
      }
      else
      {
        // Search backwards in s until we find the last space.
        for (int i = s.size() - 1; i >= 0; i--)
        {
          if (s[i] == ' ')
          {
            rem = s.substr(i) + rem;
            s = s.substr(0, i);
            break;
          }
        }
      }
    }
  }

  return strs;
}

}

#endif

