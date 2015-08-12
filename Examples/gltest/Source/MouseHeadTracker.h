#pragma once

#include <EventListener.h>
#include <Quaternion.h>

namespace Amju
{ 
// Emulates head tracking using the mouse
class MouseHeadTracker : public EventListener
{
public:
  bool Init();

  // If successful, returns true and sets q to the rotation of the HMD.
  bool Update(Quaternion* q);

  virtual bool OnCursorEvent(const CursorEvent&) override;

  void Reset();

private:
  Quaternion m_quat;
};
}
