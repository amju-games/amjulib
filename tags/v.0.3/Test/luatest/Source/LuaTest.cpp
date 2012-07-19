#ifdef WIN32
#ifdef _DEBUG
#pragma comment(lib, "../../../../../../Build/Debug/AmjuLibMsvc.lib")
#else
#pragma comment(lib, "../../../../../../Build/Release/AmjuLibMsvc.lib")
#endif 
#endif // WIN32

#include <iostream>
#include <string>
#include <Lua.h>

namespace Amju
{ 
void ReportError(const std::string& e)
{
  std::cout << e.c_str() << "\n";
}
}
using namespace Amju;

int main(int argc, char** argv)
{
  Lua lua;

  if (!lua.LoadScript("test.lua"))
  {
    std::cout << "Failed to load lua script.\n";
    exit(1);
  }

  std::cout << "Loaded script ok.\n";

  Lua::LuaFuncName funcName = "hello";
  Variable args;
  Variable retvals;

  if (!lua.Call(funcName, args, &retvals))
  {
    std::cout << "Failed to call function\n";
    exit(1);
  }

  std::cout << "Supposedly called function\n";


  std::cout << "Finished.\n";
  return 0;
}



