#pragma once

#include <Singleton.h>
#include "GSTemple.h"

namespace Amju 
{
class GSFireTemple : public GSTemple
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

};
typedef Singleton<GSFireTemple> TheGSFireTemple;
} // namespace

