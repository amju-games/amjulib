#ifndef BILLBOARD_H
#define BILLBOARD_H

#include "SceneNode.h"
#include "AmjuGL.h"
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

protected:
  float m_size; // side length of square billboard
  PTexture m_texture;
  AmjuGL::Tris m_tris;
};
}

#endif
