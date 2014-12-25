#pragma once

#include <Quaternion.h>

namespace Amju
{
// Oculur VR Head Tracker.
// Generates Rotation events from OVR head tracking.
class OvrHeadTracker
{
public:
  bool Init();

  // If successful, returns true and sets q to the rotation of the HMD.
  bool Update(Quaternion* q);
};

}

