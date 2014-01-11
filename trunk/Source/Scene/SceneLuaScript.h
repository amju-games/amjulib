#pragma once

#include <SceneNode.h>
#include <AmjuLua.h>

namespace Amju
{
// Calls Lua script in Update, to generate new value for local matrix.
class SceneLuaScript : public SceneNode
{
public:
  SceneLuaScript();
  virtual void Update() override;
  bool LoadScript(const std::string& scriptFilename);

protected:
  Lua m_lua;
};
}

