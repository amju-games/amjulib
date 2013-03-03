#ifndef CAM_2D_H_INCLUDED
#define CAM_2D_H_INCLUDED

#include <SceneNodeCamera.h>
#include "Ve1Object.h"

namespace Amju
{
class Cam2d : public SceneNodeCamera
{
public:
  Cam2d();
  void SetTarget(Ve1Object* g) { m_target = g; }
  virtual void Update();

  void Reset();

private:
  Ve1Object* m_target;
};
}

#endif

