#ifndef SKYBOX_H
#define SKYBOX_H

#include "Ve1Object.h"
#include "ObjMesh.h"

namespace Amju
{
class SceneMesh;

class Skybox : public Ve1Object
{
public:
  static const char* TYPENAME;

  Skybox();

  virtual const char* GetTypeName() const;
  virtual bool Load(File*);
  virtual void Update();
  virtual void OnLocationEntry();
  virtual void SetEditMenu(GuiMenu*);

private:
  // TODO Visible meshes - bg and clouds, rotating at different rates..?
  PObjMesh m_mesh; //???
  float m_xRot;
  float m_yRot;
};
}

#endif
