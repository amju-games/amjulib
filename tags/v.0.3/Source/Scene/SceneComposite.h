#ifndef SCENE_COMPOSITE_H
#define SCENE_COMPOSITE_H

#include "SceneNode.h"

namespace Amju
{
class SceneComposite : public SceneNode
{
public:

  virtual void Draw();
  virtual void Update();
  virtual bool Load(File*);
  virtual void CombineTransform();
  virtual void UpdateBoundingVol();

  void AddChild(PSceneNode node);

protected:
  typedef std::vector<PSceneNode> Nodes;
  Nodes m_children;
};
}

#endif
