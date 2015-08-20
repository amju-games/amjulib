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
  m_levelFilename = FIRE_TEMPLE_LEVEL;
  m_loadedOk = false;
  m_name = "Fire Temple";
  m_description = "Zelda OOT Fire Temple";
}

void GSFireTemple::Update()
{
  GSTemple::Update();

  float dt = TheTimer::Instance()->GetDt();
  m_pos += m_vel * dt;

  TheGame::Instance()->UpdateGameObjects();
//  GetSceneGraph()->Update();
}

void GSFireTemple::DrawScene()
{
  GSTemple::DrawScene();

  float dt = TheTimer::Instance()->GetDt();

  // TODO Flickering light node for fire temple
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

/*
  // Draw axes
  AmjuGL::SetColour(Colour(1, 1, 1, 1));
  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);
  AmjuGL::DrawLine(AmjuGL::Vec3(-1000, 0, 0), AmjuGL::Vec3(1000, 0, 0));
  AmjuGL::DrawLine(AmjuGL::Vec3(0, -1000, 0), AmjuGL::Vec3(0, 1000, 0));
  AmjuGL::DrawLine(AmjuGL::Vec3(0, 0, -1000), AmjuGL::Vec3(0, 0, 10000));
  AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
*/
}

bool GSFireTemple::OnKeyEvent(const KeyEvent& ke)
{
  if (GSTemple::OnKeyEvent(ke))
  {
    return true;
  }
  return false;
}

void GSFireTemple::OnActive()
{
  GSTemple::OnActive();

}

} // namespace
