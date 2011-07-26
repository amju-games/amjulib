/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: UrlUtils.cpp,v $
Revision 1.8  2008/05/18 16:04:32  jay
MSVC update

Revision 1.7  2008/05/08 10:58:08  jay
New memory management code

Revision 1.6  2007/01/13 16:19:58  jay
Fix for uploading binary data: convert ALL characters to hex-encoded format.

Revision 1.5  2007/01/03 09:05:04  jay
Add Overrides ofToUrlFormat

Revision 1.4  2006/12/04 21:49:38  jay
Added funcs to strip data from URL

Revision 1.3  2006/10/27 22:23:44  jay
Merge improvements from Pool, for HTTP online stuff.

Revision 1.1.2.2  2006/07/27 08:42:46  jay
Added ToUrlFormat(), to correctly format all characters in a URL.

Revision 1.1.2.1  2005/10/21 17:51:59  jay
All URL-manipulation functions moved here; copied over from Uptime Overlord.

Revision 1.2  2005/02/28 20:34:24  jay
Additions and fixes for Uptime Overlord v.1.0 release

Revision 1.1  2005/01/03 11:08:55  jay
Added to Uptime Overlord project

*/

#ifdef WIN32
#pragma warning(disable: 4786)
#endif
#include "AmjuFirst.h"
#include <iostream>
#include "UrlUtils.h"
#include "StringUtils.h"
#include "AmjuFinal.h"

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
        c == '-'   ||
        c == '_');
}

// Replace spaces with %20, etc.
std::string ToUrlFormat(const std::string& s)
{
  std::string r;
  for (unsigned int i = 0; i < s.length(); i++)
  {
    char c = s[i];

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

std::string ToUrlFormat(const BinaryData& d)
{
  std::string r;
  const int size = d.size();
  for (int i = 0; i < size; i++)
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
  unsigned int f = url.find("?");
  if (f == std::string::npos)
  {
    return ""; // no data
  }
  return url.substr(f); // return f to end
}

std::string StripDataFromUrl(const std::string& url)
{
  unsigned int f = url.find("?");
  if (f == std::string::npos)
  {
    return url; // no data
  }
  return url.substr(0, f); 
}
}

