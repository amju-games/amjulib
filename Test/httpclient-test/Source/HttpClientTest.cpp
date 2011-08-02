/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: HttpClientTest.cpp,v $
Revision 1.6  2008/05/08 10:58:04  jay
New memory management code

Revision 1.5  2006/12/10 12:11:34  jay
Compile-time option to test POST; show error message

Revision 1.4  2006/12/04 21:50:33  jay
Test HTTP GET and POST

Revision 1.3  2006/10/27 22:23:42  jay
Merge improvements from Pool, for HTTP online stuff.

Revision 1.1.10.3  2006/08/28 22:05:25  Administrator
Win: Added Windows Sleep() call - TODO platform wrapper for sleep

Revision 1.1.10.2  2006/07/27 08:43:29  jay
Tests ToUrlFormat()

Revision 1.1.10.1  2006/07/25 08:11:17  jay
Tests for new Http classes

Revision 1.1  2004/09/08 15:43:19  jay
Added to repository
  
*/

#include "AmjuFirst.h"
#include <iostream>
#include <string>
#include "HttpClient.h"
#include "HttpReq.h"
#include "OnlineReqManager.h"
#include "SocketService.h"
#include "UrlUtils.h"

#ifdef GEKKO
#include <AmjuGL-GCube.h>
#include <EventPollerImplWii.h>
#include <Pause.h>
#include <stdlib.h>
#include <unistd.h> // sleep()
#endif

#include "AmjuFinal.h"

#define TEST_HTTP_CLIENT_GET
//#define TEST_HTTP_CLIENT_POST
//#define TEST_HTTP_REQ
//#define TEST_ONLINE_REQ_MAN

using namespace Amju;

class TestReq : public OnlineReq
{
public:
  TestReq(
    const std::string& url,
    HttpClient::HttpMethod method,
    const std::string& name) : 
    OnlineReq(url, method, name)
  {
  }

  virtual void HandleResult()
  {
    std::cout << "\n\nTestReq has finished.\n";
    if (GetResult().GetSuccess())
    {
      std::cout << "SUCCESS! Got result: " 
        << GetResult().GetString().c_str() << "\n";
    }
    else
    {
      std::cout << "FAILED: " << GetResult().GetErrorString().c_str() << "\n";
    }
  }
};


void TestGet(const std::string& url)
{
  HttpClient hc;

  std::cout << "Testing GET..\n";

  HttpResult res = hc.Get(url, HttpClient::GET);

  if (res.GetSuccess())
  {
    std::cout << res.GetString().c_str() << "\n";
  }
  else
  {
    std::cout << "Failed. Error: " << res.GetErrorString().c_str() << "\n";
  }
}

void TestPost(const std::string& url)
{
  HttpClient hc;

  std::cout << "Testing POST..\n";
  
  HttpResult res = hc.Get(url, HttpClient::POST);
  
  if (res.GetSuccess())
  {
    std::cout << res.GetString().c_str() << "\n";
  }
  else
  {
    std::cout << "Failed. Error: " << res.GetErrorString().c_str() << "\n";
  }
}

void TestHttpReq(const std::string& url)
{
  std::cout << "Testing HttpReq..\n";

  HttpReq req(url, HttpClient::GET);
  std::cout << "waiting";
 
  // Uhh.. WTF, this isn't a separate thread ?!
  while (!req.IsFinished())
  {
    std::cout << ".";
    sleep(1);
  } 
  std::cout << "FINISHED";
  std::cout << "\n";
  std::cout << req.GetResult().GetString().c_str() << "\n";
}

void TestOnlineReq(const std::string& url)
{
  OnlineReqManager rm;
  TestReq* pReq = new TestReq(url, HttpClient::GET, "test-request");
  rm.AddReq(pReq);
  while (true)
  {
    rm.Update();
    std::cout << ".";
    std::cout.flush();
#if defined (MACOSX) || defined (GEKKO)
    sleep(1);
#else
    Sleep(1000);
#endif
  }
}

int main(int argc, char** argv)
{
  AMJU_CALL_STACK;

#ifdef GEKKO
  AmjuGL::SetImpl(new AmjuGLGCubeConsole);
  AmjuGL::Init();
  TheEventPoller::Instance()->SetImpl(new EventPollerImplWii);
#endif

  std::cout << "\n\n\n\n\nHello! I R HttpClientTest\n\n";

  if (argc != 2)
  {
    std::cout << "Usage: HttpClientTest <url> \n";
    PAUSE;
    return 1;
  }

  std::string url = argv[1];

  std::cout << "Raw URL: " << url.c_str() << "\n";
  std::cout << "URL format: " << ToUrlFormat(url).c_str() << "\n";

//  TestGet(url);
  TestPost(url);
//  TestHttpReq(url);
//  TestOnlineReq(url);

  PAUSE;
  return 0;
}

