#pragma once

#include <SceneNode.h>
#include <AmjuLua.h>

namespace Amju
{
// Calls Lua script in Update, to generate new value for local matrix.
class SceneLuaScript : public SceneNode
{
public:
  static const char* NAME;
  static SceneNode* Create();

  SceneLuaScript();
  virtual void Update() override;
  virtual bool Load(File*) override;

  bool LoadScript(const std::string& scriptFilename);

protected:
  Lua m_lua;
};
}

