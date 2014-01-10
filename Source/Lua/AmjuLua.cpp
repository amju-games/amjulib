#include <AmjuFirst.h>
#include <iostream>
extern "C"
{
#include "LuaLib-5.2.3/lua.h"
#include "LuaLib-5.2.3/lualib.h"
#include "LuaLib-5.2.3/lauxlib.h"
}
#include "AmjuLua.h"
#include <File.h>
#include <Variable.h>
#include <StringUtils.h>
#include <AmjuFinal.h>

namespace Amju
{
extern void ReportError(const std::string&);

static bool s_showInfo = false;

bool Lua::ShowInfoMessages()
{
  return s_showInfo;
}

void Lua::SetShowInfoMessages(bool show)
{
  s_showInfo = show;
}

static bool ShowInfo()
{
  return s_showInfo;
}

// Push args onto Lua stack.
// Used by Call() and Return() below
// Returns no of args pushed.
// Called recursively to flatten vectors of variables
static int PushLuaArgs(lua_State* L, const Variable& v)
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
    lua_pushstring(L, v.GetString().c_str());
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
  else
  {
    // TODO
    std::cout << "LUA: c++: Type not handled yet! Get on it dude!\n";
    Assert(0);
  }

  return numPushed;
}

int Lua::Return(lua_State* L, const Variable& retVals)
{
  if (ShowInfo())
  {
    std::cout << "LUA: c++: returning these vals: " << retVals.ToString() << "\n";
  }

  // Return total number of args pushed after flattening
  return PushLuaArgs(L, retVals);
}

static Variable GetVariableFromLua(lua_State* L, int i)
{
  int t = lua_type(L, i);
  switch (t)
  {
  case LUA_TNUMBER:
    // sigh, can't distinguish between float and int numeric types?
    return Variable(float(lua_tonumber(L, i)));
    break;
  case LUA_TBOOLEAN:
    return Variable(lua_toboolean(L, i));
    break;
  case LUA_TSTRING:
    return Variable(std::string(lua_tostring(L, i)));
    break;
      
  case LUA_TTABLE:
  case LUA_TNIL:
  case LUA_TFUNCTION:
  case LUA_TUSERDATA: 
  case LUA_TTHREAD:
  case LUA_TLIGHTUSERDATA:
    std::cout << "LUA: c++: GetParams(): Type not handled yet! Get on it dude!\n";
    Assert(0); // TODO
  }
  return Variable(); // null type
}

Variable Lua::GetParams(lua_State* L)
{
  int n = lua_gettop(L);    /* number of arguments */

  VariableVec vv;
  for (int i = 1; i <= n; i++)
  {
    vv.push_back(GetVariableFromLua(L, i));
  }

  Variable v(vv);
  if (ShowInfo())
  {
    std::cout << "LUA: c++: got these params: " << v.ToString() << "\n";
  }

  return v;
}

Lua::Lua()
{
  m_pL = luaL_newstate(); // NOT lua_open()

  // Include all lua libraries.
  luaL_openlibs(m_pL);

  luaopen_debug(m_pL); // we want to make it as easy as possible to debug scripts!
}

Lua::~Lua()
{
  lua_close(m_pL);
}

bool Lua::LoadScriptFromFile(const std::string& filename, bool useGlue)
{
  m_filename = filename;

  // TODO Use glue file if required
  int r = luaL_loadfile(m_pL, filename.c_str());

  if (r == 0)
  {
    // Need to do this before we can call any functions in the file.
    lua_call(m_pL, 0, 0);
    if (ShowInfo())
    {
      std::cout << "LUA: c++: Loaded lua file " << filename << " OK\n";
    }
  }
  else
  {
    std::string error = "LUA: c++: Loading lua file \"" + filename + 
      "\" failed: Code " + ToString(r) + ": " + lua_tostring(m_pL, -1);
    ReportError(error);
  }

  return (r == 0);


  // TODO use Amju::File
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

bool Lua::LoadScriptFromMem(const std::string& script)
{
  // TODO
  return false;
}

bool Lua::Call(
  const Lua::LuaFuncName& funcName, 
  const Variable& args, 
  Variable* pResults,
  int numRetVals)
{
  if (ShowInfo())
  {
    std::cout << "LUA: c++: calling lua function \"" << funcName
      << "\" Args: " 
      << args.ToString() << "\n";
  }

  lua_getglobal(m_pL, funcName.c_str());

  // Push each arg on to the lua stack
  int numArgs = PushLuaArgs(m_pL, args);

  // Call func, give no of args and no of return values
  int retcode = lua_pcall(m_pL, numArgs, numRetVals, 0);
  if (retcode != 0)
  {
    // Pop error message off stack.
    const char* msg = lua_tostring(m_pL, -1);
    lua_pop(m_pL, 1);
    std::string err = "LUA: c++: attempt to call lua function \"" + funcName + "\" ";
    if (m_filename.empty())
    {
      err += "(no file name) ";
    }
    else
    {
      err += "in file \"" + m_filename + "\" ";
    }
    err += "failed: ";
    ReportError(err + msg);

    return false;
  }

  // Get return vals
  if (numRetVals == 0)
  {
    // Nothing to do
  }
  else if (numRetVals == 1)
  {
    Variable v = GetVariableFromLua(m_pL, -1);
    lua_pop(m_pL, 1);
    *pResults = v;
  }
  else
  {
    VariableVec vec;
    for (int j = 0; j < numRetVals; j++)
    {
      Variable v = GetVariableFromLua(m_pL, -1);
      vec.push_back(v);
      lua_pop(m_pL, 1);
    }
    std::reverse(vec.begin(), vec.end()); // !
    *pResults = vec;
  }

  if (ShowInfo())
  {
    std::cout << "LUA: c++ call to lua function \"" << funcName << "\" succeeded. Return values: " 
      << pResults->ToString() << "\n";
  }
 
  return true;
}

bool Lua::Register(const Lua::LuaFuncName& funcName, lua_CFunction func)
{
  const char* funcNameStr = funcName.c_str();
  lua_register(m_pL, funcNameStr, func);

  if (ShowInfo())
  {
    std::cout << "LUA: c++ function \"" << funcName << "\" registered OK\n"; 
  }

  return true;
}
}

