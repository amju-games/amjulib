// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026

#include "precomp.h" // first include
#include <AmjuFirst.h>
#ifdef WIN32
#pragma warning(disable: 4786)
#endif
#include <iostream>
#include "UrlUtils.h"
#include <StringUtils.h>
#include <Directory.h>
#include <File.h>

#ifdef ANDROID_NDK
#include <ctype.h>
#endif

#include <AmjuFinal.h>

namespace Amju
{
bool IsUrlPrintable(char c)
{
  AMJU_CALL_STACK;

    return (isalnum(c) || 
        c == '.'   ||
        c == '='   ||
        c == '/'   ||
        c == '\\'   ||
        c == '&'   ||
        c == '?'   ||
        c == '+'   ||
////        c == '-'   ||
        c == '_');
}

// Replace spaces with %20, etc.
std::string ToUrlFormat(const std::string& s)
{
  std::string r;
  for (unsigned int i = 0; i < s.length(); i++)
  {
    unsigned char c = s[i];

    if (IsUrlPrintable(c))
    {
      r += std::string(1, c);
    }
    else
    {
      // Replace c with e.g.  %20 - i.e. '%' followed by c in hex.
      r += std::string(1, '%');
      r += ToHexString(c);
    }
  }
  return r;
}

std::string ToUrlFormat(const BinaryData& d)
{
  std::string r;
  const auto size = d.size();
  for (auto i = 0; i < size; i++)
  {
    unsigned char c = d[i];
    if (IsUrlPrintable(c))
    {
      r += std::string(1, c);
    }
    else
    {
      // Replace c with e.g.  %20 - i.e. '%' followed by c in hex.
      r += std::string(1, '%');
      r += ToHexString((unsigned int)c);
    }
  }
  return r;
}

std::string ToUrlFormat(const unsigned char* const data, int numbytes)
{
  std::string r;
  for (int i = 0; i < numbytes; i++)
  {
    unsigned char c = (char)data[i];
    
    // Replace c with e.g.  %20 - i.e. '%' followed by c in hex.
    r += std::string(1, '%');
    r += ToHexString((unsigned int)c);

/*
    if (IsUrlPrintable(c))
    {
      r += std::string(1, c);
    }
    else
    {
      // Replace c with e.g.  %20 - i.e. '%' followed by c in hex.
      r += std::string(1, '%');
      r += ToHexString((unsigned int)c);
    }
*/
  }

  return r;
}

std::string GetServerNameFromUrl(const std::string& url)
{
  std::string u(url);
  // Remove starting "http://" if it exists
  u = Replace(u, "http://", "");

  // Remove all after and including the first slash
  if (u.find("/") != std::string::npos)
  { 
    u.erase(u.begin() + u.find("/"), u.end());
  }

  // Remove all after and including colon
  if (u.find(":") != std::string::npos)
  {
    u.erase(u.begin() + u.find(":"), u.end());
  }

  return u;
}

std::string GetPathFromUrl(const std::string& url)
{
  std::string u(url);
  // Remove starting "http://" if it exists
  u = Replace(u, "http://", "");

  // Remove all up to and NOT including the first slash
  if (url.find("/") == std::string::npos)
  {
    return "/";
  }
  u.erase(u.begin(), u.begin() + u.find("/"));

  return u;
}

int GetPortFromUrl(const std::string& url)
{
  AMJU_CALL_STACK;

  // If url ends with a colon followed by a number, this specifies the port.
  // Otherwise default to 80.
  std::string u(url);
  // Remove starting "http://" if it exists
  u = Replace(u, "http://", "");

  // Remove all after and including the first slash
  if (u.find("/") != std::string::npos)
  {
    u.erase(u.begin() + u.find("/"), u.end());
  }

  int p = 80;
 
  if (u.find(":") != std::string::npos)
  {
    std::string strp = u.substr(u.find(":") + 1);
    if (!u.empty())
    {
      p = atoi(strp.c_str());
    }
  }

  return p;
}

std::string GetDataFromUrl(const std::string& url)
{
  // Strip characters before the first '?'
  std::string::size_type f = url.find("?");
  if (f == std::string::npos)
  {
    return ""; // no data
  }
  return url.substr(f + 1); // return f to end - STRIP THE "?"
}

std::string StripDataFromUrl(const std::string& url)
{
  std::string::size_type f = url.find("?");
  if (f == std::string::npos)
  {
    return url; // no data
  }
  return url.substr(0, f); 
}

bool FileContentToUrl(const std::string& filename, std::string* result)
{
  if (!FileExists(filename))
  {
#ifdef FC_DEBUG
std::cout << "FileContentToUrl: Apparently there is no file '"
  << filename.c_str() << "'\n";
#endif
    return false;
  }

  File file(false, File::STD);
  if (!file.OpenRead(filename, true, false))
  {
#ifdef FC_DEBUG
std::cout << "FileContentToUrl: failed to open file '"
  << filename.c_str() << "'\n";
#endif
    return false;
  }

#ifdef FC_DEBUG
std::cout << "FileContentToUrl: Opened file '" << filename.c_str() << "'...\n";
#endif

  static const unsigned int BUF_SIZE = 4096; // TODO CONFIG

  unsigned char data[BUF_SIZE];
  // Read file until we get to the end.
  unsigned int total = 0;
  while (true)
  {
    unsigned int bytesRead = file.GetBinary(BUF_SIZE, data);
    total += bytesRead;

#ifdef FC_DEBUG
std::cout << "FileContentToUrl:  ..read " << bytesRead << " bytes, total: "
  << total << "\n";
#endif

    *result += ToUrlFormat(data, bytesRead);
    if (bytesRead < BUF_SIZE)
    {
      break;
    }
  }

#ifdef FC_DEBUG
std::cout << "FileContentToUrl:  success!\n";
#endif
  return true;
}

std::string EncodeStr(const std::string& plainMsg)
{
  std::string result;
  
  int s = static_cast<int>(plainMsg.size());
  for (int i = 0; i < s; i++)
  {
    unsigned char c = plainMsg[i];
    result += ToHexString(c);
  }
  
  return result;
}

std::string DecodeStr(const std::string& encodedMsg)
{
  std::string result;
  
  int s = static_cast<int>(encodedMsg.size());
  for (int i = 0; i < s; i += 2)
  {
    std::string hex = encodedMsg.substr(i, 2);
    int n = UIntFromHexString(hex);
    result += std::string(1, (char)n);
  }
  
  return result;
}
}

