#ifndef SCENE_GAME_OBJECT_H
#define SCENE_GAME_OBJECT_H

// TODO Move to Game module ?

#include "SceneComposite.h"
#include "GameObject.h" 

namespace Amju
{
// Draws a GameObject 
// All SceneNodes have children, e.g. Shadow
class SceneGameObject : public SceneNode
{
public:
  SceneGameObject(PGameObject pGo);

  virtual void UpdateBoundingVol();

protected:
  PGameObject m_pGo;
};

class SceneGameObjectOpaque : public SceneGameObject
{
public:
  SceneGameObjectOpaque(PGameObject pGo);
  virtual void Draw();
};

class SceneGameObjectBlended : public SceneGameObject
{
public:
  SceneGameObjectBlended(PGameObject pGo);
  virtual void Draw();
};
}

#endif

