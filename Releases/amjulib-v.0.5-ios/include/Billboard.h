#ifndef BILLBOARD_H
#define BILLBOARD_H

#include "SceneNodeProcMesh.h"

namespace Amju
{
// Currently recalcs mesh on CPU.
// TODO Recalc vertec positions in shader.
class Billboard : public SceneNodeProcMesh
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
};
}

#endif

