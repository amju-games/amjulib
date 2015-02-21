#include <EventPoller.h>
#include "MouseHeadTracker.h"

namespace Amju
{
  static const float CONVERT_MOUSE_DX_TO_RADIANS = 1.0f;
  static const float CONVERT_MOUSE_DY_TO_RADIANS = 1.0f;

  bool MouseHeadTracker::Init()
  {
    TheEventPoller::Instance()->AddListener(this);
    return true;
  }

  void MouseHeadTracker::Reset()
  {
    // Reset rotations
    m_quat = Quaternion();
  }

  bool MouseHeadTracker::Update(Quaternion* q)
  {
    *q = m_quat;
    return true;
  }

  bool MouseHeadTracker::OnCursorEvent(const CursorEvent& ce)
  {
    const Vec3f Y_AXIS(0, 1, 0);
    const Vec3f X_AXIS(1, 0, 0);
    Quaternion xRot(Y_AXIS, (float)ce.dx * -CONVERT_MOUSE_DX_TO_RADIANS);
    Quaternion yRot(X_AXIS, (float)ce.dy *  CONVERT_MOUSE_DY_TO_RADIANS);
    m_quat = xRot * m_quat;

    return false; // don't consume event
  }
}