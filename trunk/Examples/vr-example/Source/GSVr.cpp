#include <AmjuGL.h>
#include <Camera.h>
#include <DegRad.h>
#include <GLUtils.h>
#include <Teapot.h>
#include <Screen.h>
#include <Vec3.h>
#include <Quaternion.h>
#include <ResourceManager.h>
#include <File.h>
#include <Timer.h>
#include <ReportError.h>
#include <StereoDraw.h>
#include "GSVr.h"

#define AMJU_USE_OVR

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

static bool symmetrical = true;

static float vpXOffset = 0.08;
static float vpYOffset = 0.04;

// Fire temple scale
static const float INITIAL_EYESEP = 0.35f;

static const Vec3f ORIG_VIEW_DIR(0, 0, -1);
static const Vec3f ORIG_UP_DIR(0, 1, 0);

/*  
struct Camera
{
  float neardist;
  float fardist;
  float aperture; // fov x
  float eyesep;
  float fo; // WHAT IS THIS?
  Vec3f pos, dir, up;
};
*/

Camera theCamera;

GSVr::GSVr()
{
  // Initial look dir
  m_viewDir = ORIG_VIEW_DIR;
  m_upDir = ORIG_UP_DIR;
  
  m_mesh = 0;
}

void GSVr::Update()
{
#ifdef AMJU_USE_OVR
  // Get Oculus Rift rotation info
  // TODO Pass absolute time as second param?
  // j.c.
  ovrSensorState ss = ovrHmd_GetSensorState(hmd, 0);
  const ovrPoseStatef& ps = ss.Recorded; // Predicted;
  const ovrPosef& pose = ps.Pose;
  const ovrQuatf& oquat = pose.Orientation;
  Amju::Quaternion q(oquat.w, oquat.x, oquat.y, oquat.z);
  m_viewDir = q.RotateVec(ORIG_VIEW_DIR);
  m_upDir = q.RotateVec(ORIG_UP_DIR);
#endif

  float dt = TheTimer::Instance()->GetDt();
  m_pos += m_vel * dt;
}

static ObjMesh* theMesh = 0;
void DrawScene()
{
  Vec3f pos(1, 1, 1);
  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING); // mesh normals are no good 
  
  theMesh->Draw();
}

void DrawStereo()
{
  static StereoDraw sd;
  sd.SetCamera(theCamera);
  sd.SetDrawFunc(DrawScene);
  sd.Draw();
}

void GSVr::Draw()
{
  theMesh = m_mesh;

  theCamera.m_pos = m_pos;
  theCamera.m_dir = m_viewDir;
  theCamera.m_up = m_upDir;

  //m_barrel.Draw();

  DrawStereo();

  AmjuGL::UseShader(0);
  AmjuGL::SetClearColour(Colour(0, 0, 0, 1));
}

bool GSVr::OnKeyEvent(const KeyEvent& ke)
{
  const float WALK_SPEED = 2.0f;

  if (ke.keyType == AMJU_KEY_UP)
  {
    if (ke.keyDown)
    {
      m_vel = m_viewDir * WALK_SPEED;
    }
    else
    {
      m_vel = Vec3f(0, 0, 0); // TODO decelerate to a stop?
    }
  }

  if (ke.keyType == AMJU_KEY_DOWN)
  {
    if (ke.keyDown)
    {
      m_vel = m_viewDir * -WALK_SPEED;
    }
    else
    {
      m_vel = Vec3f(0, 0, 0); // TODO decelerate to a stop?
    }
  }

  if (ke.keyType == AMJU_KEY_RIGHT)
  {
    if (ke.keyDown)
    {
      Vec3f cameraright = CrossProduct(m_viewDir, m_upDir);
      m_vel = cameraright * WALK_SPEED;
    }
    else
    {
      m_vel = Vec3f(0, 0, 0); // TODO decelerate to a stop?
    }
  }

  if (ke.keyType == AMJU_KEY_LEFT)
  {
    if (ke.keyDown)
    {
      Vec3f cameraright = CrossProduct(m_viewDir, m_upDir);
      m_vel = cameraright * -WALK_SPEED;
    }
    else
    {
      m_vel = Vec3f(0, 0, 0); // TODO decelerate to a stop?
    }
  }

  // Focal distance
  if (ke.keyType == AMJU_KEY_CHAR && ke.key == 'a' && ke.keyDown)
  {
    theCamera.m_fo += 2.0f; // ?
    std::cout << "fo: " << theCamera.m_fo << "\n";
  }
  else if (ke.keyType == AMJU_KEY_CHAR && ke.key == 'd' && ke.keyDown)
  {
    theCamera.m_fo -= 2.0f; // ?
    std::cout << "fo: " << theCamera.m_fo << "\n";
  }

  // Eye sep
  if (ke.keyType == AMJU_KEY_CHAR && ke.key == 'z' && ke.keyDown)
  {
    theCamera.m_eyeSep += 0.01f; // ?
    std::cout << "eyesep: " << theCamera.m_eyeSep << "\n";
  }
  else if (ke.keyType == AMJU_KEY_CHAR && ke.key == 'c' && ke.keyDown)
  {
    theCamera.m_eyeSep -= 0.01f; // ?
    if (theCamera.m_eyeSep < 0)
    {
        theCamera.m_eyeSep = 0;
    }
    std::cout << "eyesep: " << theCamera.m_eyeSep << "\n";
  }

  // Symmetrical/asymmetrical
  if (ke.keyType == AMJU_KEY_CHAR && ke.key == 's' && ke.keyDown)
  {
      symmetrical = !symmetrical;
      std::cout << "Now using " << (symmetrical ? "symmetrical" : "asymmetrical") << " frusta.\n";
  }

  // VIEWPORT OFFSET
  if (ke.keyType == AMJU_KEY_CHAR && ke.key == 'o' && ke.keyDown)
  {
      vpXOffset += 0.01f;
      std::cout << "VP X offset: " << vpXOffset << "\n";
  }
  else if (ke.keyType == AMJU_KEY_CHAR && ke.key == 'p' && ke.keyDown)
  {
      vpXOffset -= 0.01f;
      std::cout << "VP X offset: " << vpXOffset << "\n";
  }

  if (ke.keyType == AMJU_KEY_CHAR && ke.key == 'k' && ke.keyDown)
  {
      vpYOffset += 0.01f;
      std::cout << "VP Y offset: " << vpYOffset << "\n";
  }
  else if (ke.keyType == AMJU_KEY_CHAR && ke.key == 'l' && ke.keyDown)
  {
      vpYOffset -= 0.01f;
      std::cout << "VP Y offset: " << vpYOffset << "\n";
  }
  
  return true;
}

bool GSVr::OnRotationEvent(const RotationEvent& re)
{
  // Rotate view and up vecs by rotation
  
  float a = re.degs * 1.0f; 
  
  static float x = 0;
  static float y = 0;
  static float z = 0;
  
  switch (re.axis)
  {
    case AMJU_AXIS_X:
    {
#if 1
      // Y-rot
      y = a;
#endif
      break;
    }
    case AMJU_AXIS_Y:
    {
#if 1
      // This works - z axis - TODO check 90 deg turn is calibrated correctly
      z = a;
#endif
      break;
    }
    case AMJU_AXIS_Z:
    {
#if 1
      // This works! - x axis
      x = a * 2.0f; // needs more rotation??
#endif
      break;
    }
      
  }
  
  Amju::Quaternion qx(Vec3f(1, 0, 0),  x);
  Amju::Quaternion qy(Vec3f(0, 1, 0),  y);
  Amju::Quaternion qz(Vec3f(0, 0, -1), z);
  Amju::Quaternion q =  qy * qx; // * qz;
  
  m_viewDir = q.RotateVec(ORIG_VIEW_DIR);
  m_upDir = q.RotateVec(ORIG_UP_DIR);

std::cout << "Got rotation event\n";
  
  return true;
}
  
void GSVr::Draw2d()
{
}

#ifdef AMJU_USE_OVR
static bool InitOVR()
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

    return true;
}
#endif // AMJU_USE_OVR

void GSVr::OnActive()
{
#ifdef AMJU_USE_OVR
  // TODO Run time switch?
  InitOVR();
#endif
  
#ifdef WIN32
  // TODO TEMP TEST
  File::SetRoot("C:\\jay\\projects\\amjulib\\Examples\\ovr-example\\Assets\\fire_temple", "/");
#endif

  ResourceManager* rm = TheResourceManager::Instance();
  rm->AddLoader("obj", TextObjLoader);
  m_mesh = (ObjMesh*)rm->GetRes("model.obj");
  Assert(m_mesh);

  theCamera.m_neardist = 1.0f;
  theCamera.m_fardist = 200.0f; // TODO Get size of env? Up to some max
  theCamera.m_fovy = 1.0f; // TODO TEST radians
  theCamera.m_eyeSep = INITIAL_EYESEP; // TODO TEST
  theCamera.m_fo = 75.0f; 

  m_pos.y = 10;

  // Set up barrel dist post process effect
  m_barrel.SetDrawFunc(DrawStereo);
  m_barrel.Init();
}

} // namespace
