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
#include "GSFireTemple.h"

namespace Amju
{
GSFireTemple::GSFireTemple()
{
  m_mesh = nullptr;
  m_name = "Fire Temple";
  m_description = "Zelda OOT Fire Temple";
}

void GSFireTemple::Update()
{
  GSBase::Update();

  float dt = TheTimer::Instance()->GetDt();
  m_pos += m_vel * dt;
}

void GSFireTemple::DrawScene()
{
  DrawHelp(); 
  m_mesh->Draw();
}

bool GSFireTemple::OnKeyEvent(const KeyEvent& ke)
{
  if (GSBase::OnKeyEvent(ke))
  {
    return true;
  }

/*
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
*/

  return true;
}

void GSFireTemple::OnActive()
{
  GSBase::OnActive();

  ResourceManager* rm = TheResourceManager::Instance();
  rm->AddLoader("obj", TextObjLoader);
  m_mesh = (ObjMesh*)rm->GetRes("fire_temple/model.obj");
  Assert(m_mesh);

//  theCamera.m_neardist = 1.0f;
//  theCamera.m_fardist = 200.0f; // TODO Get size of env? Up to some max
//  theCamera.m_fovy = 1.0f; // TODO TEST radians
//  theCamera.m_eyeSep = INITIAL_EYESEP; // TODO TEST
//  theCamera.m_fo = 75.0f; 

  m_camera.m_pos.y = 10;

  // Set up barrel dist post process effect
//  m_barrel.SetDrawFunc(DrawStereo);
//  m_barrel.Init();
}

} // namespace
