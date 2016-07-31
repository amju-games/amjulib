#pragma once

#include <string>
#include <RCPtr.h>
#include <Variable.h>

struct lua_State;
typedef int (*lua_CFunction)(lua_State*);

namespace Amju
{
// C++ functions used in this Lua wrapper. 
typedef Variable (*WrappedLuaFunc)(const Variable&);
// You write a function with the above sig. Create a wrapper function with AMJU_MAKE_LUA_WRAPPER
// E.g. 
// Variable AwesomeFunc(const Variable& params) { ... } 
// AMJU_MAKE_LUA_WRAPPER(AwesomeFunc)


// Wrapper around Lua API.
// We want to load lua scripts, call functions in the scripts, and register
// C functions for the scripts to call.
//
// Current design idea is that you subclass this; create static functions 
// for the C(++) functions you want to expose to lua scripts, and register
// the static functions in the subclass ctor.
//
// Expected use:
//  - Subclass, registering new functions; or, compose with a Lua object, and
//    register functions explicitly.
//  - Call an 'Update' lua function each game loop if required.
//  - Call new lua functions when game events occur. These lua functions will
//    call C(++) functions which were registered in step 1.
// This should enable an event-driven framework where the actual game 
// mechanics are in lua scripts, but all the graphics, physics, OS interaction,
// etc is handled by the Amju C++ engine.
//
class Lua : public RefCounted
{
public:
  // Right now lua function names are just strings. It might make more sense
  // to have a type standing for functions. That way we would never give the
  // wrong string, rely on const values, etc. The downside would be that we 
  // would have to pass the function objects around all the time. Hmm.
  typedef std::string LuaFuncName;

  Lua();

  // Closes the lua state member. That is why currently copying is forbidden;
  // the alternative would be to use ref counting.
  virtual ~Lua();

  // Flag to see information about function calls - off by default
  static void SetShowInfoMessages(bool);
  static bool ShowInfoMessages();

  // Load a lua script from a file. You can disable glue loading for user-
  // supplied scripts not in any glue file. If the glue file is enabled
  // (the default), then the current glue file will be used, if glue
  // file loading has been turned on globally.
  bool LoadScriptFromFile(const std::string& filename, bool enableGlue = true);

  // Load a (text) lua script from memory.
  bool LoadScriptFromMem(const std::string& script);

  // Call a lua function in the currently loaded lua script.
  // Returns true if function call succeeded; this is NOT the return value
  // of the function. Get the return value(s) in the 3rd param.
  // We need to specify in the 4th parameter how many return values we
  //  expect - Lua can return multiple values.
  // Pass a single parameter as a Variable; pass multiple parameters 
  // as a vector of Variables; NB 
  // VariableVec params will have to be flattened; only simple types can
  // be passed to a lua function AFAIK.
  bool Call(const LuaFuncName& funcName, const Variable& args,
    Variable* pResults, int numRetVals = 1);

  // Register a C function so that lua code can call it.
  // The idea is that this would be called in a subclass ctor to register
  // application-specific functions we want lua scripts to call.
  // Or you could choose to not subclass and just explicitly register
  // functions - e.g. when a script is loaded.
  // Don't call this: use RegisterWrappedFunc instead, so you can use
  //  WrappedLuaFunc sig.
  bool Register(const LuaFuncName& funcName, lua_CFunction);

 // TODO

  // Get params as a Variable - this is to be called by C functions called
  //  from lua.
  static Variable GetParams(lua_State* L);

  // Pass return values in Variable to lua - called by C function when
  //  returning control to lua.
  // Returns total number of args pushed after flattening retvals.
  static int Return(lua_State* L, const Variable& retVals);

private:
  // Uncopyable: if we pass by value we will destroy the Lua State in the
  // dtor. Alternatively we could ref count the Lua State.
  Lua(const Lua&); // =delete
  Lua& operator=(const Lua&); // =delete

private:
  lua_State* m_pL;
  std::string m_filename; // for error reporting
};

#define AMJU_MAKE_LUA_WRAPPER(fn) \
static int fn##_wrapper(lua_State* L) \
{ \
  if (Lua::ShowInfoMessages()) { std::cout << "LUA: c++: function " << #fn << " called from lua\n"; } \
  return Lua::Return(L, fn(Lua::GetParams(L))); \
}

#define RegisterWrappedFunc(name, fn) Register(name, fn##_wrapper)

}


