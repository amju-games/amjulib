// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#if defined(MACOSX) || defined(WIN32)

#ifdef WIN32
#include <rpc.h>
#endif // WIN32

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
  *devId = reinterpret_cast<char*>(str);

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
  *devId = "mac device";
  *deviceUserName = "Mac user";
  *deviceModel = "Mac";
  *deviceOsVersion = "OS version";
#endif

  return true;
}
}

#endif