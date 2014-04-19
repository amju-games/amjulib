#include <ReportError.h>
#include <Timer.h>
#include <VariableGeomConvert.h>
#include <File.h>
#include "SceneLuaScript.h"

namespace Amju
{
const char* SceneLuaScript::NAME = "scene-lua";

SceneNode* SceneLuaScript::Create()
{
  return new SceneLuaScript;
}

SceneLuaScript::SceneLuaScript()
{
}

bool SceneLuaScript::Load(File* f) 
{
  std::string filename;
  if (!f->GetDataLine(&filename))
  {
    f->ReportError("Expected lua script file name.");
    return false;
  }
  return LoadScript(filename);
}

void SceneLuaScript::Update() 
{
  float dt = TheTimer::Instance()->GetDt();

  // Call lua update() function
  
  Lua::LuaFuncName funcName = "update";
  // Pass in Matrix and dt
  VariableVec args;
  args.push_back(ToVariable(m_local));
  args.push_back(dt);

  Variable retvals;

  // Expect 16 floats back
  if (!m_lua.Call(funcName, args, &retvals, 16))
  {
    std::cout << "Failed to call function '" << funcName << "'\n";
    Assert(0);
    return; 
  }

  // Retval is new matrix
  Assert(retvals.IsVectorType());
  Assert(retvals.GetVector().size() == 16);
  m_local = ToMatrix(retvals);

std::cout << "New matrix: " << m_local << "\n";
}

bool SceneLuaScript::LoadScript(const std::string& scriptFilename)
{
////  m_lua = new Lua;

  if (!m_lua.LoadScriptFromFile(scriptFilename))
  {
    ReportError("Failed to load lua script " + scriptFilename);
    return false;
  }

  // Register C functions to update matrix

  // Check update function exists

  return true;
}

}


