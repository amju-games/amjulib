/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: HttpClient.cpp,v $
Revision 1.12  2008/05/08 10:58:04  jay
New memory management code

Revision 1.11  2007/10/22 20:57:07  jay
Debug out compile flag

Revision 1.10  2007/10/21 22:57:19  jay
Debug out: turned off

Revision 1.9  2007/08/29 22:23:54  jay
Copy improvements from Pool - use non-leaky "MetalShell" impl, with
improved error reporting

Revision 1.1.10.13  2007/07/25 07:53:41  jay
Turn off debug out in release mode

Revision 1.1.10.12  2007/07/06 08:41:26  jay
Improved error messages (METALSHELL)

Revision 1.1.10.11  2007/05/26 21:59:34  jay
Use Metalshell impl, not leaky Amju impl - accidentally switched in last checkin

Revision 1.1.10.10  2007/05/16 22:20:23  Administrator
Fix MetalShell impl on Windows

Revision 1.1.10.9  2007/05/15 12:36:22  jay
Amju Http Request LEAKS SOCKETS! Use MetalShell impl, pending investigation

Revision 1.1.10.8  2007/03/16 08:51:29  jay
Turn off debug output

Revision 1.1.10.7  2007/03/13 00:12:02  jay
Copy latest Http code from trunk

Revision 1.8  2007/01/01 21:52:13  jay
Don't format URL/params, in case client code has already done so.
(Doing it twice results in garbage.)
This means you must explicity use ToUrlFormat() in UrlUtils.

Revision 1.7  2006/12/11 23:04:57  Administrator
MSVC fixes

Revision 1.6  2006/12/10 12:28:23  jay
Use HTTP 1.0, not 1.1, so we don't have to deal with chunked data.
Function to strip chunk info does not work properly.
Factor out MakeHeader from metalshell and Amju impls.

Revision 1.5  2006/12/04 21:51:13  jay
Implement HTTP POST

Revision 1.4  2006/10/27 22:23:42  jay
Merge improvements from Pool, for HTTP online stuff.

Revision 1.1.10.6  2006/07/30 21:34:44  jay
Optimisation: Check for end of Html and bail if found

Revision 1.1.10.5  2006/07/27 08:43:14  jay
Call ToUrlFormat()

Revision 1.1.10.4  2006/07/26 21:16:36  jay
Added alternate implementation but still suffers from big pause :-(

Revision 1.1.10.3  2006/07/25 08:10:59  jay
Uses new HttpResult
Added code to use CURL but currently unused and hopefully will not be needed

Revision 1.1.10.2  2005/10/21 17:50:44  jay
Copied latest version over from Uptime Overlord

Revision 1.1  2005/01/03 11:08:52  jay
Added to Uptime Overlord project

Revision 1.1  2004/09/08 15:43:19  jay
Added to repository
  
*/

#include "AmjuFirst.h"

//#define HTTP_DEBUG

// Define this to use code borrowed from www.metalshell.com
#define USE_METALSHELL_CODE
//#define USE_AMJU_CODE

#ifdef USE_METALSHELL_CODE
#ifdef MACOSX
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#endif // MACOSX

#if defined(WIN32)
#include <winsock2.h>
#include <windows.h>
#endif // WIN32

#ifdef GEKKO
#include <network.h>
#include <stdlib.h>
#include <string.h>
#endif

#endif // USE_METALSHELL_CODE

#include <iostream>
#include "HttpClient.h"
#include "SocketService.h"
#include "ClientSocket.h"
#include "StringUtils.h"
#include "UrlUtils.h"
#include "AmjuFinal.h"

#ifdef USE_METALSHELL_CODE
#define PROTOCOL "tcp"
#define SERVICE "http"
//#define GET "GET / HTTP/1.0\n\n"
#endif

// For intro to http see: http://www.jmarshall.com/easy/http/

// TODO 
// Make sure this always returns
// Pauses for about 5 seconds when all data has been received
// Report useful error messages:
// - bad server name
// - no internet connection

namespace Amju
{
void HttpResult::SetString(const std::string& str)
{
  AMJU_CALL_STACK;

  m_str = str;
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

std::string HttpResult::GetString() const
{
  return m_str;
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

std::string MakeHeader(
  HttpClient::HttpMethod m, const std::string& path, const std::string& host)
{
  std::string getReqStr;

  if (m == HttpClient::GET)
  {
    getReqStr = "GET " + path + " HTTP/1.0\r\nHost: " + host + "\r\n\r\n";
  }
  else if (m == HttpClient::POST)
  {
    std::string data = GetDataFromUrl(path).substr(1);
    if (!data.empty())
    {
      data = data.substr(0); // lose '?'
    }
    std::string noDataPath = StripDataFromUrl(path); 

    getReqStr = "POST " + noDataPath + " HTTP/1.0\r\nHost: " + host + "\r\n";
    getReqStr += "Content-Length: ";
    getReqStr += ToString((int)data.size());
    getReqStr += "\r\n";
    getReqStr += "Content-Type: application/x-www-form-urlencoded"; 
    getReqStr += "\r\n\r\n";
    getReqStr += data;
  }

  return getReqStr;
}

void StripChunkInfo(std::string* str)
{
  AMJU_CALL_STACK;

  if (!StringContains(*str, "Transfer-Encoding: chunked"))
  {
    // Not chunked, so nothing to do
#ifdef HTTP_DEBUG
std::cout << "NOT CHUNKED\n";
#endif

    return;
  }

  typedef std::vector<std::string> LineVec;
  LineVec v = Split(*str, '\n');

#ifdef HTTP_DEBUG
  {
    std::cout << "Stripping chunks: " << v.size() << " lines:\n";
    for (int i = 0; i < v.size(); i++)
    {
      std::cout << "\"" << v[i] << "\"\n";
    } 
  }
#endif

  // Find the first blank line, so we know we have found the start
  // of the message body.
  unsigned int i = 0;
  while (i < v.size())
  {
    std::string s = v[i];
    Trim(&s);
    if (s.empty())
    {
      std::cout << "Found blank line\n";
      i++;
      break;
    }
    i++;
  }
  
  while (i < v.size())
  {
    // Read the chunk:
    // Read the first line: this is of the form
    // <hex length of chunk>[; optional semicolon + some text to ignore]
    std::string line = v[i];

#ifdef HTTP_DEBUG
std::cout << "Chunk line: \"" << line.c_str() << "\"\n";
#endif

    // Strip semicolon and anything following it
    // TODO

#ifdef HTTP_DEBUG
std::cout << "Stripped: \"" << line.c_str() << "\"\n";
#endif

    // Get the chunk length
    int chunkLength = strtol(line.c_str(), 0, 16); // NB Base 16 

#ifdef HTTP_DEBUG
std::cout << "Chunk length (base 10): " << chunkLength << "\n";
#endif

    // Remove this line from str
    // TODO

    // If the chunk length is zero, this is the final chunk
    if (chunkLength == 0)
    {
      break;
    }

    // Skip lines until we have skipped <chunkLength> characters
    int charsSkipped = 0;
    ++i;
    while (charsSkipped < chunkLength && i < v.size())
    {
      std::string line = v[i];
      charsSkipped += line.size(); 

#ifdef HTTP_DEBUG
std::cout << "  Skipping line \"" << line.c_str() << "\" chars now: " 
  << charsSkipped << "\n";
#endif

      ++i;
    }
  }

  // Finally there may be footers.  
  // These could be moved up to the header to make it easier to parse
  // the reply.
  // TODO
}

#ifdef USE_METALSHELL_CODE
HttpResult MetalshellGet(
  HttpClient::HttpMethod m,
  const std::string& path,
  const std::string& host,
  int port)
{
  AMJU_CALL_STACK;

#ifdef HTTP_DEBUG
std::cout << "METALSHELL IMPLEMENTATION\n";
#endif

 int sockid;
 int bufsize;
 char buffer[32768]; //1024];
 struct sockaddr_in socketaddr;
 struct hostent *hostaddr;
 //struct servent *servaddr;
 //struct protoent *protocol;

 /* Resolve the host name */
#ifdef GEKKO
 if (!(hostaddr = net_gethostbyname(host.c_str()))) 
#else
 if (!(hostaddr = gethostbyname(host.c_str()))) 
#endif
 {
    //fprintf(stderr, "Error resolving host.");
    HttpResult r; 
    r.SetSuccess(false);
    std::string s = "Failed to find server " + host;
    r.SetErrorString(s);
    return r;
 }

 /* clear and initialize socketaddr */
 memset(&socketaddr, 0, sizeof(socketaddr));
 socketaddr.sin_family = AF_INET;

 /* setup the servent struct using getservbyname */
//#ifdef GEKKO
// servaddr = net_getservbyname(SERVICE, PROTOCOL);
//#else
// servaddr = getservbyname(SERVICE, PROTOCOL);
//#endif

 unsigned short usport = port;

 // Hmm, not sure if this used to work properly, it just seemed to look up the port number
 // for the http service, instead of using the port parameter.. yikes
 socketaddr.sin_port = htons(usport); //servaddr->s_port;

 memcpy(&socketaddr.sin_addr, hostaddr->h_addr, hostaddr->h_length);

 /* protocol must be a number when used with socket()
    since we are using tcp protocol->p_proto will be 0 */
// protocol = getprotobyname(PROTOCOL);

#ifdef GEKKO
 sockid = net_socket(AF_INET, SOCK_STREAM, 0); //protocol->p_proto);
#else
 sockid = socket(AF_INET, SOCK_STREAM, 0); //protocol->p_proto);
#endif
 if (sockid < 0) {
    //fprintf(stderr, "Error creating socket.");
    HttpResult r; 
    r.SetSuccess(false);
    std::string s = "Failed to create socket, connecting to server " + host + ":" +
      ToString(port);
    r.SetErrorString(s);
    return r;
 }

 /* everything is setup, now we connect */
#ifdef GEKKO
 if(net_connect(sockid, (sockaddr*)&socketaddr, sizeof(socketaddr)) == -1) 
#else
 if(connect(sockid, (sockaddr*)&socketaddr, sizeof(socketaddr)) == -1) 
#endif
 {
    //fprintf(stderr, "Error connecting.");
    HttpResult r; 
    r.SetSuccess(false);
    std::string s = "Failed to connect to server " + host + ":" +
      ToString(port);
    r.SetErrorString(s);
    return r;
 }

 /* send our get request for http */
 std::string getReqStr = MakeHeader(m, path, host);

#ifdef HTTP_DEBUG
std::cout << "Req str: '" << getReqStr.c_str() << "'\n";
#endif

#ifdef GEKKO
 if (net_send(sockid, getReqStr.c_str(), getReqStr.size(), 0) == -1) 
#else
 if (send(sockid, getReqStr.c_str(), getReqStr.size(), 0) == -1) 
#endif
 {
    //fprintf(stderr, "Error sending data.");
    HttpResult r; 
    r.SetSuccess(false);
    std::string s = "Failed to send request to server " + host + ":" +
      ToString(port);
    r.SetErrorString(s);
    return r;
 }

  HttpResult r; 
  r.SetSuccess(true);
  std::string result;

 /* read the socket until its clear then exit */
#if defined(WIN32)
 while ( (bufsize = recv(sockid, buffer, sizeof(buffer) - 1, 0))) 
#elif defined(GEKKO)
 while ( (bufsize = net_read(sockid, buffer, sizeof(buffer) - 1))) 
#else
 while ( (bufsize = read(sockid, buffer, sizeof(buffer) - 1))) 
#endif
 {
//   write(1, buffer, bufsize);
#ifdef HTTP_DEBUG
 std::cout << "Got this: " << buffer << "\n";
#endif

  buffer[bufsize] = '\0';
  result += buffer;  
 }

 StripChunkInfo(&result);

  r.SetString(result);

#if defined(WIN32)
  closesocket(sockid);
#elif defined (GEKKO)
  net_close(sockid);
#else
  close(sockid);
#endif

  return r;
}
#endif

#ifdef USE_AMJU_CODE
HttpResult AmjuGet(
  HttpClient::HttpMethod m,
  const std::string& path,
  const std::string& host,
  int port)
{
  AMJU_CALL_STACK;

#ifdef HTTP_DEBUG
std::cout << "Path: " << path << "\n";
std::cout << "Server: " << host << "\n";
std::cout << "Port: " << port << "\n";
#endif

  ClientSocket s;

  if (!s.Connect(host, port))
  {
#ifdef HTTP_DEBUG
    std::cout << "Failed to connect to " << host << " port " << port << "\n";
#endif
 
    HttpResult r; 
    // TODO failure enum
    r.SetSuccess(false);
    std::string s = "Failed to connect to server " + host + ":" +
      ToString(port);
    r.SetErrorString(s);
    return r;
  }

  std::string request = MakeHeader(m, path, host);

#ifdef HTTP_DEBUG
std::cout << "Sending request:\n" << request << "\n";
#endif

  if (!s.WriteRawString(request))
  {
#ifdef HTTP_DEBUG
    std::cout << "Problem sending request.\n";
#endif
    
    HttpResult r; 
    // TODO failure enum
    r.SetSuccess(false);
    r.SetErrorString("Failed to send request to server");
    return r;
  }

#ifdef HTTP_DEBUG
  std::cout << "Sent request. Waiting for reply...\n";
#endif

  std::string totalReply;
  std::string reply;

  while (s.GetRawString(&reply) && !reply.empty())
  {
    totalReply += reply;   

    // Check reply for "</html>" -- if we have this we can bail out - 
    // this is for loading html only, don't forget.
    // This could prevent the long pause when ther is no more data to load.
    // TODO Case-insensitive StringContains()

    if (StringContains(reply, "</html>") || 
        StringContains(reply, "</HTML>"))
    {
      break;
    }
    // HACK for APZ 
    if (StringContains(reply, "</apz>"))
    {
      break;
    }
    // HACK for pool online
    if (StringContains(reply, "</ascp>"))
    {
      break;
    }
  }

  s.Close();

  StripChunkInfo(&totalReply);

  HttpResult r;
  r.SetString(totalReply);
  r.SetSuccess(true);

  return r;
}
#endif

HttpResult HttpClient::Get(
  const std::string& url, 
  HttpClient::HttpMethod m)
{
  AMJU_CALL_STACK;

  TheSocketService::Instance(); // make sure we are initialised

  std::string path = GetPathFromUrl(url);
  std::string host = GetServerNameFromUrl(url);
  int port = GetPortFromUrl(url);

std::cout << "Path: " << path << "\n";
std::cout << "Server: " << host << "\n";
std::cout << "Port: " << port << "\n";

#ifdef USE_METALSHELL_CODE
  return MetalshellGet(m, path, host, port);
#endif

#ifdef USE_AMJU_CODE
  return AmjuGet(m, path, host, port);
#endif
}
 
}

