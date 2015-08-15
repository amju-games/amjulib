#include <AmjuGL.h>
#include <Camera.h>
#include <DegRad.h>
#include <File.h>
#include <Game.h>
#include <GLUtils.h>
#include "GSFireTemple.h"
#include "GSLoadLevel.h"
#include "MySceneGraph.h"
#include <ResourceManager.h>
#include <StereoDraw.h>
#include <Timer.h>

// Hmm, maybe move all assets including this to Assets/firetemple/ subdir
#define FIRE_TEMPLE_LEVEL "level-firetemple.txt"

namespace Amju
{
GSFireTemple::GSFireTemple()
{
  m_mesh = nullptr;
  m_loadedOk = false;
  m_name = "Fire Temple";
  m_description = "Zelda OOT Fire Temple";
}

void GSFireTemple::Update()
{
  GSBase::Update();

  float dt = TheTimer::Instance()->GetDt();
  m_pos += m_vel * dt;

  TheGame::Instance()->UpdateGameObjects();
//  GetSceneGraph()->Update();
}

void GSFireTemple::DrawScene()
{
  //DrawHelp(); 

  if (!m_loadedOk)
    return;

  GetSceneGraph()->GetCamera()->SetFromCamera(m_camera);
  GetSceneGraph()->Draw();

/*
  static FireTexture ft;
  static Billboard bb;
  static bool first = true;
  if (first)
  {
    first = false;
    //ft.Init();

    bb.SetTexture(ft.GetTexture());
    bb.SetSize(1.0f);
    Vec3f pos(0, 8, 0);
    Matrix m;
    m.Translate(pos);
    bb.SetLocalTransform(m);
    bb.CombineTransform();
  }
*/

  float dt = TheTimer::Instance()->GetDt();

  static float t = 0;
  if (!m_paused)
  {
      t += dt;
  }
  Vec3f pos(cos(t), 1, sin(t));

  AmjuGL::Enable(AmjuGL::AMJU_LIGHTING);
  AmjuGL::DrawLighting(
      AmjuGL::LightColour(0, 0, 0),
      AmjuGL::LightColour(0.2f, 0.2f, 0.2f), // Ambient light colour
      AmjuGL::LightColour(1, 1, 1), // Diffuse light colour
      AmjuGL::LightColour(1, 1, 1),
      AmjuGL::Vec3(pos.x, pos.y, pos.z)); // Light direction

  ////m_mesh->Draw();

  // Draw axes
  AmjuGL::SetColour(Colour(1, 1, 1, 1));
  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);
  AmjuGL::DrawLine(AmjuGL::Vec3(-1000, 8, 0), AmjuGL::Vec3(1000, 8, 0));
  AmjuGL::DrawLine(AmjuGL::Vec3(0, -1000, 0), AmjuGL::Vec3(0, 1000, 0));
  AmjuGL::DrawLine(AmjuGL::Vec3(0, 0, -1000), AmjuGL::Vec3(0, 0, 10000));
  AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);

  ////ft.Update();

  ////AmjuGL::Enable(AmjuGL::AMJU_BLEND);
  ////bb.Draw();
}

bool GSFireTemple::OnKeyEvent(const KeyEvent& ke)
{
  if (GSBase::OnKeyEvent(ke))
  {
    return true;
  }
  return false;
}

void GSFireTemple::OnActive()
{
  GSBase::OnActive();

  m_loadedOk = false;
  GSLoadLevel* LL = TheGSLoadLevel::Instance();
  if (LL->GetLastLoadedLevel() != FIRE_TEMPLE_LEVEL)
  {
    LL->SetLevelFile(FIRE_TEMPLE_LEVEL);
    LL->SetPrevState(this);
    TheGame::Instance()->SetCurrentState(LL);
    return;
  }
  m_loadedOk = true;

  // TODO This will also be a game object?
  ResourceManager* rm = TheResourceManager::Instance();
  rm->AddLoader("obj", TextObjLoader);
  m_mesh = (ObjMesh*)rm->GetRes("fire_temple/model.obj");
  Assert(m_mesh);

//  theCamera.m_neardist = 1.0f;
//  theCamera.m_fardist = 200.0f; // TODO Get size of env? Up to some max
//  theCamera.m_fovy = 1.0f; // TODO TEST radians
//  theCamera.m_eyeSep = INITIAL_EYESEP; // TODO TEST
//  theCamera.m_fo = 75.0f; 

  m_camera.m_pos.y = 0;

  // Set up barrel dist post process effect
//  m_barrel.SetDrawFunc(DrawStereo);
//  m_barrel.Init();
}

} // namespace
