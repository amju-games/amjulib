// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026
#if !defined(HTTP_CLIENT_H_INCLUDED)
#define HTTP_CLIENT_H_INCLUDED

#include <string>
#include <vector>
#ifdef AMJU_USE_CURL
#include "curl/curl.h"
#endif // AMJU_USE_CURL

namespace Amju
{
class HttpResult
{
public:
  void AppendData(const unsigned char* data, int numbytes);
  void SetSuccess(bool);
  void SetErrorString(const std::string& errorStr);

  const std::string& GetString() const; 

  const unsigned char* GetData() const;
  unsigned int Size() const;

  // Return true if we got a reponse - but that's not to say it was a success..
  bool GetSuccess() const;
  std::string GetErrorString() const;

  // Get HTTP code, e.g. 404 for not found, 200 for OK, etc.
  // Returns 0 if no code can be found in the response.
  int GetHttpResponseCode() const;

private:
  std::string m_data;
  friend class HttpClient;

  bool m_success;
  std::string m_errorStr;
};


class HttpClient
{
public:
  enum HttpMethod { GET, POST };

  HttpClient();
  ~HttpClient();

  // NB This url is NOT formatted i.e. characters changed to %<hex number>
  // Do this yourself if required, using UrlUtils functions.
  bool Get(const std::string& url, HttpMethod m, HttpResult* result);

  static void SetProxy(const std::string& proxyName, int port, const std::string& user, const std::string& pw);

protected:
#ifdef AMJU_USE_CURL
  CURL* m_curl;
#endif // AMJU_USE_CURL
};

}

#endif

