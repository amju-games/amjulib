#include <AmjuGL.h>
#include <Teapot.h>
#include <Screen.h>
#include <Vec3.h>
#include <Quaternion.h>
#include <ResourceManager.h>
#include <File.h>
#include <Timer.h>
#include <DegRad.h>
#include "GSVr.h"

#include <OVR.h>
using namespace OVR;

namespace Amju
{
static ovrHmd hmd;
ovrHmdDesc hmdDesc;

bool symmetrical = true;

float vpXOffset = 0.08;
float vpYOffset = 0.04;

// Fire temple scale
const float INITIAL_EYESEP = 0.35f;

const Vec3f ORIG_VIEW_DIR(0, 0, -1);
const Vec3f ORIG_UP_DIR(0, 1, 0);
  
struct Camera
{
  float neardist;
  float fardist;
  float aperture; // fov x
  float eyesep;
  float fo; // WHAT IS THIS?
  Vec3f pos, dir, up;
};

Camera theCamera;

// Replace glFrustum, TODO put in AmjuGL utils
void Frustum(float left, float right, float bottom, float top, float zNear, float zFar)
{
  // Column major matrix
  float m[16];
  // See man glFrustum :-)
  m[0] = 2.0f * zNear / (right - left);
  m[1] = 0;
  m[2] = 0;
  m[3] = 0;
  
  m[4] = 0;
  m[5] = 2 * zNear / (top - bottom);
  m[6] = 0;
  m[7] = 0;

  m[8] = (right + left) / (right - left);
  m[9] = (top + bottom) / (top - bottom);
  m[10] = - (zFar + zNear) / (zFar - zNear);
  m[11] = -1;
  
  m[12] = 0;
  m[13] = 0;
  m[14] = - (2.0f * zFar * zNear) / (zFar - zNear);
  m[15] = 0;

  AmjuGL::MultMatrix(m);
}

GSVr::GSVr()
{
  // Initial look dir
  m_viewDir = ORIG_VIEW_DIR;
  m_upDir = ORIG_UP_DIR;
  
  m_mesh = 0;
}

void GSVr::Update()
{
  // Get Oculus Rift rotation info
  // TODO Pass absolute time as second param?
  // j.c.
  //ovrSensorState ss = ovrHmd_GetSensorState(hmd, 0);
  //const ovrPoseStatef& ps = ss.Recorded; // Predicted;
  //const ovrPosef& pose = ps.Pose;
  //const ovrQuatf& oquat = pose.Orientation;
  //Amju::Quaternion q(oquat.w, oquat.x, oquat.y, oquat.z);
  //m_viewDir = q.RotateVec(ORIG_VIEW_DIR);
  //m_upDir = q.RotateVec(ORIG_UP_DIR);

  float dt = TheTimer::Instance()->GetDt();
  m_pos += m_vel * dt;
}

enum Eye { LEFT, RIGHT };

void SetViewport(Eye eye, float vpX, float vpY)
{
    const int windowwidth = Screen::X();
    const int windowheight = Screen::Y();

    if (eye == LEFT)
    {
        AmjuGL::Viewport(0 + vpX, 0 + vpY,
            windowwidth / 2 - vpX - 2,
            windowheight - vpY * 2);
    }
    else
    {
        AmjuGL::Viewport(windowwidth / 2 + 4, 0 + vpY,
            windowwidth / 2 - 2 - vpX,
            windowheight - vpY * 2);
    }
}

void SetUpCameraSymmetric(Eye eye, const Camera& camera)
{
    const int windowwidth = Screen::X();
    const int windowheight = Screen::Y();

    float aspectratio = (float)windowwidth / (float)windowheight / 2.0f;
    // Dividing by 2 for side-by-side stereo

    Vec3f cameraright = CrossProduct(camera.dir, camera.up);  // Each unit vectors
    cameraright *= camera.eyesep / 2.0f;

    AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
    AmjuGL::SetIdentity();
    AmjuGL::SetPerspectiveProjection(RadToDeg(camera.aperture) / aspectratio, aspectratio,
        camera.neardist, camera.fardist);

    SetViewport(eye, vpXOffset * windowwidth, vpYOffset * windowheight);

    if (eye == LEFT)
    {
        AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
        AmjuGL::SetIdentity();
        AmjuGL::LookAt(camera.pos.x - cameraright.x, camera.pos.y - cameraright.y, camera.pos.z - cameraright.z,
            camera.pos.x - cameraright.x + camera.dir.x,
            camera.pos.y - cameraright.y + camera.dir.y,
            camera.pos.z - cameraright.z + camera.dir.z,
            camera.up.x, camera.up.y, camera.up.z);
    }
    else
    {
        AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
        AmjuGL::SetIdentity();
        AmjuGL::LookAt(camera.pos.x + cameraright.x, camera.pos.y + cameraright.y, camera.pos.z + cameraright.z,
            camera.pos.x + cameraright.x + camera.dir.x,
            camera.pos.y + cameraright.y + camera.dir.y,
            camera.pos.z + cameraright.z + camera.dir.z,
            camera.up.x, camera.up.y, camera.up.z);
    }
}

void SetUpCameraAsymmetric(Eye eye, const Camera& camera)
{
  // http://paulbourke.net/stereographics/stereorender/

  const int windowwidth = Screen::X();
  const int windowheight = Screen::Y();

  float aspectratio = (float)windowwidth / (float)windowheight / 2.0f;         
  // Dividing by 2 for side-by-side stereo

  float widthdiv2   = camera.neardist * tan(camera.aperture / 2.0f); 
  // aperture in radians
  Vec3f cameraright = CrossProduct(camera.dir,camera.up);  // Each unit vectors
  cameraright *= camera.eyesep / 2.0f;

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::SetIdentity();

  SetViewport(eye, vpXOffset * windowwidth, vpYOffset * windowheight);

  if (eye == LEFT)
  {
    float top    =   widthdiv2;
    float bottom = - widthdiv2;
    float left   = - aspectratio * widthdiv2 + 0.5f * camera.eyesep * camera.neardist / camera.fo;
    float right  =   aspectratio * widthdiv2 + 0.5f * camera.eyesep * camera.neardist / camera.fo;
    Frustum(left,right,bottom,top,camera.neardist,camera.fardist);

    AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
    AmjuGL::SetIdentity();
    AmjuGL::LookAt(camera.pos.x - cameraright.x,camera.pos.y - cameraright.y,camera.pos.z - cameraright.z,
            camera.pos.x - cameraright.x + camera.dir.x,
            camera.pos.y - cameraright.y + camera.dir.y,
            camera.pos.z - cameraright.z + camera.dir.z,
            camera.up.x,camera.up.y,camera.up.z);
  }
  else
  {
    float top    =   widthdiv2;
    float bottom = - widthdiv2;
    float left   = - aspectratio * widthdiv2 - 0.5f * camera.eyesep * camera.neardist / camera.fo;
    float right  =   aspectratio * widthdiv2 - 0.5f * camera.eyesep * camera.neardist / camera.fo;
    Frustum(left,right,bottom,top,camera.neardist,camera.fardist);

    AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
    AmjuGL::SetIdentity();
    AmjuGL::LookAt(camera.pos.x + cameraright.x,camera.pos.y + cameraright.y,camera.pos.z + cameraright.z,
            camera.pos.x + cameraright.x + camera.dir.x,
            camera.pos.y + cameraright.y + camera.dir.y,
            camera.pos.z + cameraright.z + camera.dir.z,
            camera.up.x,camera.up.y,camera.up.z); 
  }
}

void GSVr::DrawScene()
{
  Vec3f pos(1, 1, 1);
  AmjuGL::Enable(AmjuGL::AMJU_LIGHTING);
  
  AmjuGL::DrawLighting(
    AmjuGL::LightColour(0, 0, 0),
    AmjuGL::LightColour(0.2f, 0.2f, 0.2f), // Ambient light colour
    AmjuGL::LightColour(1, 1, 1), // Diffuse light colour
    AmjuGL::LightColour(1, 1, 1),
    AmjuGL::Vec3(pos.x, pos.y, pos.z)); // Light direction

  static Teapot tp;
  tp.Draw();
  
  m_mesh->Draw();
}

void GSVr::Draw()
{
  AmjuGL::SetClearColour(Colour(0, 0, 0, 1));
 
  theCamera.pos = m_pos;
  theCamera.dir = m_viewDir;
  theCamera.up = m_upDir;

  if (symmetrical)
  {
      SetUpCameraSymmetric(LEFT, theCamera);
      DrawScene();

      SetUpCameraSymmetric(RIGHT, theCamera);
      DrawScene();
  }
  else
  {
      SetUpCameraAsymmetric(LEFT, theCamera);
      DrawScene();

      SetUpCameraAsymmetric(RIGHT, theCamera);
      DrawScene();
  }

  // Restore viewport  
  const int w = Screen::X();
  const int h = Screen::Y();
  AmjuGL::Viewport(0, 0, w, h);
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
    theCamera.fo += 2.0f; // ?
    std::cout << "fo: " << theCamera.fo << "\n";
  }
  else if (ke.keyType == AMJU_KEY_CHAR && ke.key == 'd' && ke.keyDown)
  {
    theCamera.fo -= 2.0f; // ?
    std::cout << "fo: " << theCamera.fo << "\n";
  }

  // Eye sep
  if (ke.keyType == AMJU_KEY_CHAR && ke.key == 'z' && ke.keyDown)
  {
    theCamera.eyesep += 0.01f; // ?
    std::cout << "eyesep: " << theCamera.eyesep << "\n";
  }
  else if (ke.keyType == AMJU_KEY_CHAR && ke.key == 'c' && ke.keyDown)
  {
    theCamera.eyesep -= 0.01f; // ?
    if (theCamera.eyesep < 0)
    {
        theCamera.eyesep = 0;
    }
    std::cout << "eyesep: " << theCamera.eyesep << "\n";
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
  
  return true;
}
  
void GSVr::Draw2d()
{
}

bool InitOVR()
{
    ovr_Initialize();

    hmd = ovrHmd_Create(0);
    if (!hmd)
    {
        MessageBoxA(NULL, "Oculus Rift not detected.", "", MB_OK);
        return false;
    }

    //Get more details about the HMD
    ovrHmd_GetDesc(hmd, &hmdDesc);
    if (hmdDesc.DisplayDeviceName[0] == '\0')
    {
        MessageBoxA(NULL, "Rift detected, display not enabled.", "", MB_OK);
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

void GSVr::OnActive()
{
//  InitOVR();

  File::SetRoot("C:\\jay\\projects\\amjulib\\Examples\\ovr-example\\Assets\\fire_temple", "/");
  ResourceManager* rm = TheResourceManager::Instance();
  rm->AddLoader("obj", TextObjLoader);
  m_mesh = (ObjMesh*)rm->GetRes("model.obj");
  Assert(m_mesh);

  theCamera.neardist = 1.0f;
  theCamera.fardist = 200.0f; // TODO Get size of env? Up to some max
  theCamera.aperture = 1.0f; // TODO TEST radians
  theCamera.eyesep = INITIAL_EYESEP; // TODO TEST
  theCamera.fo = 75.0f; 

}

} // namespace
