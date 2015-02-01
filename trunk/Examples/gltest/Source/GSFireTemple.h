#pragma once

#include <Singleton.h>
#include <ObjMesh.h>
#include "GSBase.h"

namespace Amju 
{
class GSFireTemple : public GSBase
{
  GSFireTemple();
  friend class Singleton<GSFireTemple>;

public:
  virtual void Update() override;
  virtual void DrawScene() override;
  virtual void OnActive() override;
  
  // Movement - TODO use messages
  virtual bool OnKeyEvent(const KeyEvent&) override;

private:
  Vec3f m_pos;
  Vec3f m_vel;

  ObjMesh* m_mesh;
};
typedef Singleton<GSFireTemple> TheGSFireTemple;
} // namespace

