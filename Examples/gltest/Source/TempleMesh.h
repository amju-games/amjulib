#pragma once

#include <GameObject.h>
#include <WithSceneNode.h>

namespace Amju
{
class TempleMesh : public GameObject, public WithSceneNode
{
public:
  static const char* NAME;
  const char* GetTypeName() const override;
  bool Load(File*) override;
  void Update() override;

protected:
  SceneGraph* GetSceneGraph() override;

};
}


