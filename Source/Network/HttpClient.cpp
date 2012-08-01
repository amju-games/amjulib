/*
Amju Games source code (c) Copyright Jason Colman 2004
*/

#include <AmjuFirst.h>

#include <iostream>
#include <stdio.h>
#include <string>
#include <curl/curl.h>
#include "HttpClient.h"
#include <UrlUtils.h>
#include <StringUtils.h>
#include <AmjuAssert.h>
#include <AmjuFinal.h>

//#define HTTP_DEBUG

namespace Amju
{
// Callback - curl has data for me!
size_t callback_func(void *ptr, size_t size, size_t count, void *mydata)
{
  const char* data = (const char*)ptr;
  std::string* str = (std::string*)mydata;

  str->append(data, data + size * count);

  return size * count;
}

void HttpResult::SetSuccess(bool b)
{
  AMJU_CALL_STACK;

  m_success = b;
}

void HttpResult::SetErrorString(const std::string& errorStr)
{
  AMJU_CALL_STACK;

  m_errorStr = errorStr;
}

unsigned int HttpResult::Size() const
{
  return m_data.size();
}

const unsigned char* HttpResult::GetData() const
{
  return (const unsigned char*)m_data.data();
}

const std::string& HttpResult::GetString() const
{
  return m_data; //std::string((const char*)m_data.data(), (const char*)m_data.data() + m_data.size());
}

bool HttpResult::GetSuccess() const
{
  AMJU_CALL_STACK;

  return m_success;
}

std::string HttpResult::GetErrorString() const
{
  return m_errorStr;
}


static std::string s_proxyName;
static int s_proxyPort = -1;
static std::string s_proxyUser;
static std::string s_proxyPw;

bool HttpClient::Get(
  const std::string& url, 
  HttpClient::HttpMethod m,
  HttpResult* result)
{
  AMJU_CALL_STACK;

#ifdef HTTP_DEBUG
  {
    std::string path = GetPathFromUrl(url);
    std::string host = GetServerNameFromUrl(url);
    int port = GetPortFromUrl(url);

std::cout << "Path: " << path << "\n";
std::cout << "Server: " << host << "\n";
std::cout << "Port: " << port << "\n";
  }
#endif

  bool ok = true;
  CURL* curl = curl_easy_init();
  if(curl) 
  {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); 
    curl_easy_setopt(curl, CURLOPT_HEADER, 1);  // we DO want http header

    // For proxy:
    std::string proxy = s_proxyName;
    if (s_proxyPort != -1)
    {
      proxy += ":" + ToString(s_proxyPort);
    }
    curl_easy_setopt(curl, CURLOPT_PROXY, proxy.c_str());
    if (!s_proxyUser.empty())
    {
      std::string user = s_proxyUser;
      if (!s_proxyPw.empty()) 
      {
        user += ":" + s_proxyPw;
      }
      curl_easy_setopt(curl, CURLOPT_PROXYUSERPWD, user.c_str());
    }

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback_func);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &(result->m_data)); // Use HttpClient* ?

    // Perform the request, res will get the return code 
    CURLcode res = curl_easy_perform(curl);
    if(res != CURLE_OK)
    {
      ok = false;
      result->SetErrorString(curl_easy_strerror(res));
    }

    curl_easy_cleanup(curl);
  }
  else
  {
    ok = false;
std::cout << "UTTERLY FAILED TO GET " << url  << "\n";
    Assert(0); // ?
  }
  result->SetSuccess(ok);
  return ok;
}

void HttpClient::SetProxy(
  const std::string& proxyName, int port, const std::string& user, const std::string& pw)
{
  s_proxyName = proxyName;
  s_proxyPort = port;
  s_proxyUser = user;
  s_proxyPw = pw;
}
 
}

