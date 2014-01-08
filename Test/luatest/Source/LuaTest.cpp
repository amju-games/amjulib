#ifdef WIN32
#ifdef _DEBUG
#pragma comment(lib, "../../../../../../Build/Debug/AmjuLibMsvc.lib")
#else
#pragma comment(lib, "../../../../../../Build/Release/AmjuLibMsvc.lib")
#endif 
#endif // WIN32

#include <iostream>
#include <string>
#include <AmjuLua.h>

namespace Amju
{
// Function which we register so Lua script can call it:
// TOOD add to Amju Lua so we can more easily register functions, and don't have
//  to know what to return, how to get params, etc.
static int MyCFunc(lua_State* L)
{
  std::cout << "Good god! This is MyCFunc, in C++ code!\n";
  return 0;
}

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
    std::cout << "Failed to call function '" << funcName << "'\n";
    exit(1);
  }

  std::cout << "Supposedly called function\n";

  // Register a C++ function and get the Lua script to call it
  if (!lua.Register("mycfunc", MyCFunc))
  {
    // Failed to register function
    std::cout << "Failed to register function with Lua state\n";
    exit(1);
  }

  funcName = "TestMyRegisteredFunction";
  if (!lua.Call(funcName, args, &retvals))
  {
    std::cout << "Testing registed function: failed to call function '" << funcName << "'\n";
    exit(1);
  }

  std::cout << "Finished.\n";
  return 0;
}



