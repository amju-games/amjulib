#include <AmjuFirst.h>
#include <iostream>
extern "C"
{
#include "LuaLib-5.1.4/lualib.h"
#include "LuaLib-5.1.4/lauxlib.h"
}
#include "AmjuLua.h"
#include <File.h>
#include <AmjuFinal.h>

#define LUA_DEBUG

namespace Amju
{
extern void ReportError(const std::string&);

Lua::Lua()
{
  m_pL = lua_open();

  // Include all lua libraries.
  luaL_openlibs(m_pL);

  luaopen_debug(m_pL); // we want to make it as easy as possible to 
    // debug scripts!
}

Lua::~Lua()
{
  lua_close(m_pL);
}

bool Lua::LoadScript(const char* filename, bool useGlue)
{
#ifdef LUA_DEBUG
std::cout << "Loading lua script " << filename << "...\n";
#endif

  // TODO TEMP TEST
  int r = luaL_loadfile(m_pL, filename);
#ifdef LUA_DEBUG
std::cout << " ..result: " << r << "\n";
#endif

  // Need to do this before we can call any functions in the file.
  lua_call(m_pL, 0, 0);

  return (r == 0);


  bool NOVERSIONINFO = false;
  File f(NOVERSIONINFO, useGlue ? File::GLUE : File::STD);
  if (!f.OpenRead(filename)) // TODO binary ? use Root Dir ?
  {
    f.ReportError("Failed to open Lua script file.");
    return false;
  }
  // Load the whole file into memory if not a glue file.
  // If a glue file, it's in memory already, but we need the start and length.


  return false;
}

bool Lua::LoadScript(const std::string& script)
{
  // TODO
  return false;
}

// Push args onto Lua stack.
// Used by Call() below
// Returns no of args pushed.
int PushLuaArgs(lua_State* L, const Variable& v)
{
  int numPushed = 0;

  if (v.IsIntType())
  {
    int intArg = v.GetInt();
    lua_pushnumber(L, intArg);
    numPushed = 1;
  }
  else if (v.IsFloatType())
  {
    float fArg = v.GetFloat();
    lua_pushnumber(L, fArg);
    numPushed = 1;
  }
  else if (v.IsStringType())
  {
    const char* strArg = v.ToString().c_str();
    lua_pushstring(L, strArg);
    numPushed = 1;
  }
  else if (v.IsVectorType())
  {
    VariableVec vec = v.GetVector();
    int numArgs = vec.size();
    for (int i = 0; i < numArgs; i++)
    {
      const Variable& vv = vec[i];
      numPushed += PushLuaArgs(L, vv);
    }
  }
  return numPushed;
}

bool Lua::Call(
  const Lua::LuaFuncName& funcName, 
  const Variable& args, 
  Variable* pResults)
{
  // TODO We have to know the number of return vals up front.
  int numRetVals = 1; // TODO

  const char* funcNameStr = funcName.c_str();

  lua_getglobal(m_pL, funcNameStr);

  // Push each arg on to the lua stack
  int numArgs = PushLuaArgs(m_pL, args);

#ifdef LUA_DEBUG
std::cout << "LUA: calling function: " << funcNameStr 
  << " num args pushed: " << numArgs << "\nArgs: " 
  << args.ToString().c_str() << "\n";
#endif

  // Call func, give no of args and no of return values
  int retcode = lua_pcall(m_pL, numArgs, numRetVals, 0);
  if (retcode != 0)
  {
    // TODO report error ?

    // Pop error message off stack.
    const char* msg = lua_tostring(m_pL, -1);
    lua_pop(m_pL, 1);
    ReportError(msg);

    return false;
  }
  
  // Get return vals
  if (numRetVals == 0)
  {
    // Nothing to do
  }
  else if (numRetVals == 1)
  {
    // TODO handle different return types
    int retVal = (int)lua_tonumber(m_pL, -1);
    lua_pop(m_pL, 1);
    *pResults = retVal;
  }
  else
  {
    VariableVec vec;
    for (int j = 0; j < numRetVals; j++)
    {
      // TODO How can we handle other return types ?
      int retVal = (int)lua_tonumber(m_pL, -1);
      vec.push_back(Variable(retVal));
      lua_pop(m_pL, 1);
    }
    *pResults = vec;
  }

#ifdef LUA_DEBUG
std::cout << "LUA: function call succeeded, ret val(s): " 
  << pResults->ToString().c_str() << "\n";
#endif
 
  return true;
}

bool Lua::Register(const Lua::LuaFuncName& funcName, lua_CFunction func)
{
  const char* funcNameStr = funcName.c_str();
  lua_register(m_pL, funcNameStr, func);
  return true;
}
}

