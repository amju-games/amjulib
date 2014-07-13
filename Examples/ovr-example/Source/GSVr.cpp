#include <AmjuGL.h>
#include <Teapot.h>
#include <Screen.h>
#include <Vec3.h>
#include <Quaternion.h>
#include <ResourceManager.h>
#include <File.h>
#include <Timer.h>
#include "GSVr.h"
#include "OVR.h"

using namespace OVR;

namespace Amju
{
static ovrHmd Hmd;
ovrHmdDesc HmdDesc;
ovrEyeRenderDesc    EyeRenderDesc[2];
Matrix4f            Projection[2];
// Projection matrix for eye.
Matrix4f            OrthoProjection[2];
// Projection for 2D.
ovrTexture          EyeTexture[2];
Sizei               EyeRenderSize[2]; 
// Saved render eye sizes; base for dynamic 

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
  ovrSensorState ss = ovrHmd_GetSensorState(Hmd, 0);
  const ovrPoseStatef& ps = ss.Recorded; // Predicted;
  const ovrPosef& pose = ps.Pose;
  const ovrQuatf& oquat = pose.Orientation;
  Amju::Quaternion q(oquat.w, oquat.x, oquat.y, oquat.z);
  m_viewDir = q.RotateVec(ORIG_VIEW_DIR);
  m_upDir = q.RotateVec(ORIG_UP_DIR);

  float dt = TheTimer::Instance()->GetDt();
  m_pos += m_vel * dt;
}

enum Eye { LEFT, RIGHT };

void SetUpCamera(Eye eye, const Camera& camera)
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

  //  glMatrixMode(GL_PROJECTION);
  //  glLoadIdentity();
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::SetIdentity();

  if (eye == LEFT)
  {
    // Left eye
    AmjuGL::Viewport(0,0,windowwidth/2 - 2,windowheight);
    float top    =   widthdiv2;
    float bottom = - widthdiv2;
    float left   = - aspectratio * widthdiv2 + 0.5f * camera.eyesep * camera.neardist / camera.fo;
    float right  =   aspectratio * widthdiv2 + 0.5f * camera.eyesep * camera.neardist / camera.fo;
    Frustum(left,right,bottom,top,camera.neardist,camera.fardist);
    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
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
    // Right eye
    // For side by side stereo
    AmjuGL::Viewport(windowwidth/2 + 4,0,windowwidth/2 - 2,windowheight);
    float top    =   widthdiv2;
    float bottom = - widthdiv2;
    float left   = - aspectratio * widthdiv2 - 0.5f * camera.eyesep * camera.neardist / camera.fo;
    float right  =   aspectratio * widthdiv2 - 0.5f * camera.eyesep * camera.neardist / camera.fo;
    Frustum(left,right,bottom,top,camera.neardist,camera.fardist);
    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
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

//  static Teapot tp;
//  tp.Draw();
  
  m_mesh->Draw();
}

void GSVr::Draw()
{
  AmjuGL::SetClearColour(Colour(0, 0, 0, 1));
 
  // TODO Set up camera
  //float neardist;
  //float fardist;
  //float aperture; // fov x
  //float eyesep;
  //float fo; // WHAT IS THIS?
  //Vec3f pos, dir, up;
 
  theCamera.pos = m_pos;
  theCamera.dir = m_viewDir;
  theCamera.up = m_upDir;

  SetUpCamera(LEFT, theCamera);
  DrawScene();

  SetUpCamera(RIGHT, theCamera);
  DrawScene();

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

  if (ke.keyType == AMJU_KEY_CHAR && ke.key == 'z' && ke.keyDown)
  {
    theCamera.eyesep += 0.2f; // ?
    std::cout << "eyesep: " << theCamera.eyesep << "\n";
  }
  else if (ke.keyType == AMJU_KEY_CHAR && ke.key == 'c' && ke.keyDown)
  {
    theCamera.eyesep -= 0.2f; // ?
    std::cout << "eyesep: " << theCamera.eyesep << "\n";
  }

  return true;
}

bool GSVr::OnRotationEvent(const RotationEvent& re)
{
  // Rotate view and up vecs by rotation
  
  float a = re.degs * 1.0f; // TEST - this isn't really an angle, after all
  
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

void GSVr::OnActive()
{
  ovr_Initialize();

  Hmd = ovrHmd_Create(0);
  if (!Hmd)
  {
    // If we didn't detect an Hmd, create a simulated one for debugging.
    Hmd = ovrHmd_CreateDebug(ovrHmd_DK1);
    //UsingDebugHmd = true;
    if (Hmd)
    {
      std::cout << "Using debug HMD.\n";
    }
    else
    {
      std::cout << "Failed to create HMD.\n";
      Assert(0);
	  exit(0);
    }
  }

  // Get more details about the HMD.
  ovrHmd_GetDesc(Hmd, &HmdDesc);

  // TODO Try with/without?
  ovrHmd_SetEnabledCaps(Hmd, ovrHmdCap_LowPersistence |
	  ovrHmdCap_LatencyTest);

  // Start the sensor which informs of the Rift's pose and motion
  ovrHmd_StartSensor(Hmd, ovrSensorCap_Orientation |
	  ovrSensorCap_YawCorrection |
	  ovrSensorCap_Position, 0);


  File::SetRoot("C:\\jay\\projects\\amjulib\\Examples\\ovr-example\\Assets\\fire_temple", "/");
  ResourceManager* rm = TheResourceManager::Instance();
  rm->AddLoader("obj", TextObjLoader);
  m_mesh = (ObjMesh*)rm->GetRes("model.obj");

  theCamera.neardist = 1.0f;
  theCamera.fardist = 200.0f; // TODO Get size of env? Up to some max
  theCamera.aperture = 1.0f; // TODO TEST radians
  theCamera.eyesep = 2.5f; // TODO TEST
  theCamera.fo = 75.0f; // ??????

}

} // namespace
