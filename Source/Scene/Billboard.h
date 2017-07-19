#ifndef BILLBOARD_H
#define BILLBOARD_H

#include "SceneNode.h"
#include <Material.h>
#include <TriList.h>
#include <Texture.h>

namespace Amju
{
// Currently recalcs mesh on CPU.
// TODO Recalc vertec positions in shader.
class Billboard : public SceneNode
{
public:
  Billboard();
  virtual bool Load(File*);
  virtual void Draw();
  virtual void Update();

  void SetTexture(Texture*);
  void SetSize(float size);

protected:
  float m_size; // side length of square billboard

  // TODO Full Material here, then no need for m_texture
  PTexture m_texture;

  RCPtr<Material> m_material;

  // The tri list - dynamic, static version not required, right?
  RCPtr<TriListDynamic> m_triList;
};
}

#endif

