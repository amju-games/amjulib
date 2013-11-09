#ifndef BILLBOARD_H
#define BILLBOARD_H

#include "SceneNode.h"
#include <AmjuGL.h>
#include <TriList.h>
#include "Texture.h"

namespace Amju
{
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
  PTexture m_texture;
  RCPtr<TriListDynamic> m_triList;
};
}

#endif

