#include <DegRad.h>
#include <Vec3.h>
#include "AmjuGL.h"
#include "GLUtils.h"
#include "Screen.h"
#include "StereoDraw.h"

namespace Amju
{
StereoDraw::StereoDraw()
{
  m_isStereo = true;
}

/*
// TODO TEMP TEST - use Camera in Camera.h
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
*/

bool symmetrical = true;

enum Eye { LEFT, RIGHT };

void SetViewport(Eye eye, float vpX, float vpY)
{
    const int windowwidth = Screen::X();
    const int windowheight = Screen::Y();

    if (eye == LEFT)
    {
        AmjuGL::Viewport(0 + vpX, 0 + vpY,
            windowwidth / 2 - vpX,
            windowheight - vpY * 2);
    }
    else
    {
        AmjuGL::Viewport(windowwidth / 2, 0 + vpY,
            windowwidth / 2 - vpX,
            windowheight - vpY * 2);
    }
}

void SetUpCameraSymmetric(Eye eye, const Camera& camera)
{
    const int windowwidth = Screen::X();
    const int windowheight = Screen::Y();

    float aspectratio = (float)windowwidth / (float)windowheight / 2.0f;
    // Dividing by 2 for side-by-side stereo

    Vec3f cameraright = CrossProduct(camera.m_dir, camera.m_up);  // Each unit vectors
    cameraright *= camera.m_eyeSep / 2.0f;

    AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
    AmjuGL::SetIdentity();
    AmjuGL::SetPerspectiveProjection(RadToDeg(camera.m_fovy), aspectratio,
        camera.m_neardist, camera.m_fardist);

    SetViewport(eye, camera.m_vpXOffset * windowwidth, camera.m_vpYOffset * windowheight);

    if (eye == LEFT)
    {
        AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
        AmjuGL::SetIdentity();
        AmjuGL::LookAt(
            camera.m_pos.x - cameraright.x, 
            camera.m_pos.y - cameraright.y, 
            camera.m_pos.z - cameraright.z,
            camera.m_pos.x - cameraright.x + camera.m_dir.x,
            camera.m_pos.y - cameraright.y + camera.m_dir.y,
            camera.m_pos.z - cameraright.z + camera.m_dir.z,
            camera.m_up.x, camera.m_up.y, camera.m_up.z);
    }
    else
    {
        AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
        AmjuGL::SetIdentity();
        AmjuGL::LookAt(
            camera.m_pos.x + cameraright.x, 
            camera.m_pos.y + cameraright.y, 
            camera.m_pos.z + cameraright.z,
            camera.m_pos.x + cameraright.x + camera.m_dir.x,
            camera.m_pos.y + cameraright.y + camera.m_dir.y,
            camera.m_pos.z + cameraright.z + camera.m_dir.z,
            camera.m_up.x, camera.m_up.y, camera.m_up.z);
    }
}

void SetUpCameraAsymmetric(Eye eye, const Camera& camera)
{
  // http://paulbourke.net/stereographics/stereorender/

  const int windowwidth = Screen::X();
  const int windowheight = Screen::Y();

  float aspectratio = (float)windowwidth / (float)windowheight / 2.0f;
  // Dividing by 2 for side-by-side stereo

  float aperture = camera.fovx(); 
  float widthdiv2   = camera.m_neardist * tan(aperture / 2.0f);
  // aperture in radians
  Vec3f cameraright = CrossProduct(camera.m_dir,camera.m_up);  // Each unit vectors
  cameraright *= camera.m_eyeSep / 2.0f;

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::SetIdentity();

  SetViewport(eye, camera.m_vpXOffset * windowwidth, camera.m_vpYOffset * windowheight);

  if (eye == LEFT)
  {   
    float top    =   widthdiv2;
    float bottom = - widthdiv2;
    float left   = - aspectratio * widthdiv2 + 0.5f * camera.m_eyeSep * camera.m_neardist / camera.m_fo;
    float right  =   aspectratio * widthdiv2 + 0.5f * camera.m_eyeSep * camera.m_neardist / camera.m_fo;
    MultFrustum(left, right, bottom, top, camera.m_neardist, camera.m_fardist);

    AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
    AmjuGL::SetIdentity();
    AmjuGL::LookAt(
            camera.m_pos.x - cameraright.x,
            camera.m_pos.y - cameraright.y,
            camera.m_pos.z - cameraright.z,
            camera.m_pos.x - cameraright.x + camera.m_dir.x,
            camera.m_pos.y - cameraright.y + camera.m_dir.y,
            camera.m_pos.z - cameraright.z + camera.m_dir.z,
            camera.m_up.x,camera.m_up.y,camera.m_up.z);
  }
  else
  {
    float top    =   widthdiv2;
    float bottom = - widthdiv2;
    float left   = - aspectratio * widthdiv2 - 0.5f * camera.m_eyeSep * camera.m_neardist / camera.m_fo;
    float right  =   aspectratio * widthdiv2 - 0.5f * camera.m_eyeSep * camera.m_neardist / camera.m_fo;
    MultFrustum(left, right, bottom, top, camera.m_neardist, camera.m_fardist);

    AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
    AmjuGL::SetIdentity();
    AmjuGL::LookAt(
            camera.m_pos.x + cameraright.x,
            camera.m_pos.y + cameraright.y,
            camera.m_pos.z + cameraright.z,
            camera.m_pos.x + cameraright.x + camera.m_dir.x,
            camera.m_pos.y + cameraright.y + camera.m_dir.y,
            camera.m_pos.z + cameraright.z + camera.m_dir.z,
            camera.m_up.x,camera.m_up.y,camera.m_up.z);
  }
}

void SetUpCameraMono(const Camera& camera)
{
    const int windowwidth = Screen::X();
    const int windowheight = Screen::Y();
    float aspectratio = (float)windowwidth / (float)windowheight;

    AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
    AmjuGL::SetIdentity();
    AmjuGL::SetPerspectiveProjection(RadToDeg(camera.m_fovy), aspectratio,
        camera.m_neardist, camera.m_fardist);

    AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
    AmjuGL::SetIdentity();
    Vec3f cameraright = CrossProduct(camera.m_dir, camera.m_up);  
    AmjuGL::LookAt(
            camera.m_pos.x + cameraright.x,
            camera.m_pos.y + cameraright.y,
            camera.m_pos.z + cameraright.z,
            camera.m_pos.x + cameraright.x + camera.m_dir.x,
            camera.m_pos.y + cameraright.y + camera.m_dir.y,
            camera.m_pos.z + cameraright.z + camera.m_dir.z,
            camera.m_up.x,camera.m_up.y,camera.m_up.z);
}

void StereoDraw::Draw()
{
  AmjuGL::SetClearColour(Colour(0, 0, 0, 1));

  Assert(m_drawFunc);

  if (!m_isStereo)
  {
    SetUpCameraMono(m_camera);
    m_drawFunc();
    return;
  }

  if (symmetrical)
  {
      SetUpCameraSymmetric(LEFT, m_camera);
      m_drawFunc();

      SetUpCameraSymmetric(RIGHT, m_camera);
      m_drawFunc();
  }
  else
  {
      SetUpCameraAsymmetric(LEFT, m_camera);
      m_drawFunc();

      SetUpCameraAsymmetric(RIGHT, m_camera);
      m_drawFunc();
  }

  // Restore viewport
  const int w = Screen::X();
  const int h = Screen::Y();
  AmjuGL::Viewport(0, 0, w, h);
}
}

