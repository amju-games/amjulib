// * Amjulib *
// (c) Copyright 2018 Juliet Colman

#if defined(MACOSX) || defined(WIN32)

#ifdef MACOSX
#include <iostream>

#include <errno.h>
#include <unistd.h>
#include <sys/sysctl.h>
#include <sys/utsname.h>
#include <uuid/uuid.h>
#endif

#ifdef WIN32
#include <rpc.h>
#endif // WIN32

#include "AmjuHash.h"
#include "DeviceInfo.h"

namespace Amju
{
bool GetDeviceInfo(
  std::string* devId, 
  std::string* deviceUserName, 
  std::string* deviceModel, 
  std::string* deviceOsVersion)
{
#ifdef WIN32
  UUID uuid;
  UuidCreate(&uuid);
  RPC_CSTR str = nullptr;
  UuidToStringA(&uuid, &str);
  *devId = std::to_string(HashString(reinterpret_cast<char*>(str)));

  // From MSDN
  static const int INFO_BUFFER_SIZE = 256;
  char infoBuf[INFO_BUFFER_SIZE];
  DWORD  bufCharCount = INFO_BUFFER_SIZE;
  GetUserNameA(infoBuf, &bufCharCount);
  *deviceUserName = infoBuf;

  bufCharCount = INFO_BUFFER_SIZE;
  GetComputerNameA(infoBuf, &bufCharCount);
  *deviceModel = infoBuf; 

  OSVERSIONINFO osvi;
  ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
  osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  GetVersionEx(&osvi);

  *deviceOsVersion = "win" + std::to_string(osvi.dwMajorVersion) + "." + std::to_string(osvi.dwMinorVersion);
#endif

#ifdef MACOSX
  // https://stackoverflow.com/questions/933460/unique-hardware-id-in-mac-os-x
  timespec wait; 
  wait.tv_sec = 0;
  wait.tv_nsec = 0;
  const int SUCCESS = 0;
  uuid_t id; // unsigned char[16]
  if (SUCCESS == gethostuuid(id, &wait))
  {
    // Convert to string
    uuid_string_t strId;
    uuid_unparse(id, strId);
    *devId = std::to_string(HashString(strId));
  }
  else
  {
    *devId = "mac device";
  }

  // Use uname() to get more info
  utsname name;
  if (SUCCESS == uname(&name))
  {
    *deviceModel = name.machine;
    *deviceOsVersion = name.release;
  }
  else
  {
    *deviceModel = "Mac";
    *deviceOsVersion = "Version";
  }

  char* user = getenv("USER");
  if (user)
  {
    *deviceUserName = user;
  }
  else
  {
    *deviceUserName = "User";
  }
#endif // MACOSX

  return true;
}
}

#endif

