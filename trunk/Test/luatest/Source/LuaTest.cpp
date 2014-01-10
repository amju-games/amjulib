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
#include <VariableGeomConvert.h>

namespace Amju
{
Variable AwesomeFunc(const Variable& params) 
{ 
  std::cout << "I am in AwesomeFunc!\n";

  VariableVec retvals;
  retvals.push_back(Variable(std::string("I'm the first ret val, then 23.")));
  retvals.push_back(23);

  return retvals;
}
// This is to put a wrapper around the above function so we can register it with lua.
AMJU_MAKE_LUA_WRAPPER(AwesomeFunc)

// Function which we register so Lua script can call it:
// TOOD add to Amju Lua so we can more easily register functions, and don't have
//  to know what to return, how to get params, etc.
static int MyCFunc(lua_State* L)
{
  std::cout << "LUA: c++: function " << __FUNCTION__ << " called from lua\n";

  // Get parameters passed in, as Variable
  // TODO How do we know how many params? Maybe we have to pass num into
  //  this func.
  Variable params = Lua::GetParams(L);


  
  // Return values
  VariableVec retvals;
  retvals.push_back(Variable(std::string("I'm a ret val!")));
  retvals.push_back(23);

  Variable ret(retvals);

  // Return number of return vals pushed
  return Lua::Return(L, ret);
}

void ReportError(const std::string& e)
{
  std::cout << e.c_str() << "\n";
}
}
using namespace Amju;

bool test()
{
  Lua::SetShowInfoMessages(true);
  Lua lua;

  if (!lua.LoadScriptFromFile("test.lua"))
  {
    return false;
  }

  Lua::LuaFuncName funcName = "hello_ret3vals";
  
  VariableVec vec; // bah, no uniform initialisation??
  //{ Variable(123), Variable(456) };
  vec.push_back(123);
  vec.push_back(Variable(std::string("I r string")));

  // This works but makes output messy and hard to read
//  Matrix mtx;
//  vec.push_back(ToVariable(mtx));

  Variable args(vec);
  Variable retvals;

  int numRetVals = 3;
  if (!lua.Call(funcName, args, &retvals, numRetVals))
  {
    return false;
  }

  // Register a C++ function and get a Lua function to call it
  //if (!lua.Register("mycfunc", MyCFunc))
  //{
  //  return false;
  //}

  if (!lua.RegisterWrappedFunc("mycfunc", AwesomeFunc))
  {
    return false;
  }

  funcName = "TestMyRegisteredFunction";
  numRetVals = 2; 
  if (!lua.Call(funcName, args, &retvals, numRetVals))
  {
    return false;
  }

  std::cout << "Finished.\n";
  return true;
}

int main(int argc, char** argv)
{
  test();
}




