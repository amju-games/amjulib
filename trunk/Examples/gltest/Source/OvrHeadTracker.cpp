#include <ReportError.h>
#include "OvrHeadTracker.h"
#ifdef AMJU_USE_OVR
#include <OVR.h>
using namespace OVR;
#endif // AMJU_USE_OVR

namespace Amju
{
#ifdef AMJU_USE_OVR
static ovrHmd hmd;
ovrHmdDesc hmdDesc;
#endif // AMJU_USE_OVR

bool OvrHeadTracker::Init()
{
#ifdef AMJU_USE_OVR
  static bool first = true;
  if (first)
  {
    ovr_Initialize();

    hmd = ovrHmd_Create(0);
    if (!hmd)
    {
        ReportError("Oculus Rift not detected.");
        return false;
    }

    //Get more details about the HMD
    ovrHmd_GetDesc(hmd, &hmdDesc);
    if (hmdDesc.DisplayDeviceName[0] == '\0')
    {
        ReportError("Rift detected, display not enabled.");
        return false;
    }

    ovrHmd_SetEnabledCaps(hmd, ovrHmdCap_LowPersistence |
        ovrHmdCap_LatencyTest);

    // Start the sensor which informs of the Rift's pose and motion
    ovrHmd_StartSensor(hmd, ovrSensorCap_Orientation |
        ovrSensorCap_YawCorrection |
        ovrSensorCap_Position, 0);

    first = false;  
  }

#endif

  return true;
}

bool OvrHeadTracker::Update(Quaternion* q)
{
#ifdef AMJU_USE_OVR

  // Get Oculus Rift rotation info
  // TODO Pass absolute time as second param?
  ovrSensorState ss = ovrHmd_GetSensorState(hmd, 0);
  const ovrPoseStatef& ps = ss.Recorded; // Predicted;
  const ovrPosef& pose = ps.Pose;
  const ovrQuatf& oquat = pose.Orientation;
  *q = Quaternion(oquat.w, oquat.x, oquat.y, oquat.z);
  return true;

#endif

  return false;
}

}

