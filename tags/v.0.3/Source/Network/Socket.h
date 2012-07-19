/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Socket.h,v $
Revision 1.1  2004/09/08 15:43:20  jay
Added to repository
  
*/

#if !defined(AMJU_SOCKET_H_INCLUDED)
#define AMJU_SOCKET_H_INCLUDED

#if defined(WIN32)
#include <winsock2.h>
#endif

#if defined(MACOSX) || defined (IPHONE)
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h>
#endif

#include <string>

#if !defined(SOCKET)
#define SOCKET int
#endif

namespace Amju
{
class Socket 
{
public:
  Socket();
  Socket(const Socket&);
  Socket(SOCKET s);
  virtual ~Socket();

  void Close();

  // Override SerialInterface
  virtual bool WriteInteger(int i);

  virtual bool WriteString(const std::string& s);
  // Write string without prepending string length.
  bool WriteRawString(const std::string& s);

  virtual bool GetInteger(int* p);

  virtual bool GetString(std::string* p);
  // Get string without prepended length.
  bool GetRawString(std::string* p);

  virtual bool GetFloat(float*);

  virtual bool WriteFloat(float);

protected:
  bool Bind(int  portnum);

  // Read up to n bytes from socket, copy into buf.
  int ReadData(char* buf, int n);

  // Write n bytes from buf to the socket.
  int WriteData(const char* buf, int n);

protected:
  SOCKET m_socket;
};

} // namespace

#endif

