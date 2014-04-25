#include <AmjuGL.h>
#include <Game.h>
#include <Timer.h>
#include "GSVe3MyStats.h"
#include "GSVe3ViewOtherPlayers.h"
#include "GSMain.h"
#include "GS2dAvatarMod.h"
#include "GSQuitGame.h"
#include "GSVe3Guestbook.h"
#include "LocalPlayer.h"
#include "LayerGroup.h"
#include "ObjectManager.h"
#include "ObjectUpdater.h"
#include "GSWaitForNewLocation.h"
#include "LurkMsg.h"
#include "GameConsts.h"
#include "CheckForNewMessages.h"

namespace Amju
{

GSVe3MyStats::GSVe3MyStats()
{
  m_showLurk = true;
}

void GSVe3MyStats::RefreshGui()
{
  Player* p = GetLocalPlayer();
  Assert(p);
  ShowPlayer(p, m_gui);
}

void GSVe3MyStats::Update()
{
  GSGui::Update();

  m_spriteNode.Update();
  TheLurker::Instance()->Update();

}

void GSVe3MyStats::Draw()
{
  GSGui::Draw();
}

void GSVe3MyStats::Draw2d()
{
  // Draw player
  AmjuGL::PushMatrix();
  // Scale for 'breathing' effect..?
  AmjuGL::RotateX(90.0f); 
  static float f = 0;
  f += TheTimer::Instance()->GetDt();
  float s = sin(f) * 0.001f;
  AmjuGL::Translate(-0.6f, 0, -0.4f);
  AmjuGL::Scale(0.01f, 1, 0.01f);
  AmjuGL::Scale(1, 1, 1.0f + s);

  m_spriteNode.Draw();
  AmjuGL::PopMatrix();

  // Draw over character if necessary
  GSGui::Draw2d();
}

void GSVe3MyStats::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-ve3-mystats.txt");
  Assert(m_gui);

  //GetElementByName(m_gui, "see-msgs-button")->SetCommand(OnSeeMyMessages);

  Player* p = GetLocalPlayer();
  Assert(p);

  ShowPlayer(p, m_gui);
}

} // namespace
