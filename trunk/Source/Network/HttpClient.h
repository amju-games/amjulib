/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: HttpClient.h,v $
Revision 1.3  2007/01/01 21:52:14  jay
Don't format URL/params, in case client code has already done so.
(Doing it twice results in garbage.)
This means you must explicity use ToUrlFormat() in UrlUtils.

Revision 1.2  2006/10/27 22:23:42  jay
Merge improvements from Pool, for HTTP online stuff.

Revision 1.1.10.2  2006/07/25 08:09:58  jay
Fleshed out HttpResult

Revision 1.1.10.1  2005/10/21 17:50:44  jay
Copied latest version over from Uptime Overlord

Revision 1.1  2005/01/03 11:08:52  jay
Added to Uptime Overlord project

Revision 1.1  2004/09/08 15:43:19  jay
Added to repository
  
*/

#if !defined(HTTP_CLIENT_H_INCLUDED)
#define HTTP_CLIENT_H_INCLUDED

#include <string>

namespace Amju
{
class HttpResult
{
public:
  void SetString(const std::string& str);
  void SetSuccess(bool);
  void SetErrorString(const std::string& errorStr);

  std::string GetString() const;
  bool GetSuccess() const;
  std::string GetErrorString() const;

private:
  std::string m_str;
  bool m_success;
  std::string m_errorStr;
};

//typedef std::string HttpResult;

class HttpClient
{
public:
  enum HttpMethod { GET, POST, HEAD };

  // NB This url is NOT formatted i.e. characters changed to %<hex number>
  // Do this yourself if required, using UrlUtils functions.
  HttpResult Get(const std::string& url, HttpMethod m);
};
}

#endif

