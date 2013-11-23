#include <AmjuGL.h>
#include <Game.h>
#include <Timer.h>
#include "GSVe3HomePage.h"
#include "GSVe3ViewOtherPlayers.h"
#include "GSMain.h"
#include "GS2dAvatarMod.h"
#include "LocalPlayer.h"
#include "LayerGroup.h"
#include "ObjectManager.h"
#include "ObjectUpdater.h"
#include "GSWaitForNewLocation.h"
#include "LurkMsg.h"

namespace Amju
{
GSVe3HomePage::GSVe3HomePage()
{
}

void GSVe3HomePage::Update()
{
  GSGui::Update();

  m_spriteNode.Update();
  TheLurker::Instance()->Update();
}

void GSVe3HomePage::Draw()
{
  GSGui::Draw();

}

void GSVe3HomePage::Draw2d()
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

  TheLurker::Instance()->Draw();
}

static void OnChangeName()
{
}

static void OnChangeLook()
{
  GS2dAvatarMod* gs = TheGS2dAvatarMod::Instance();
  gs->SetPrevState(TheGSVe3HomePage::Instance());
  TheGame::Instance()->SetCurrentState(gs);
}

static void OnExplore()
{
  GSMain* gs = TheGSMain::Instance();
  gs->SetPrevState(TheGSVe3HomePage::Instance());

  // Get location for local player
  Player* p = GetLocalPlayer();
  Assert(p);
  int loc = p->GetLocation();
  SetLocalPlayerLocation(loc);

  TheGame::Instance()->SetCurrentState(TheGSWaitForNewLocation::Instance());
}

static void OnOtherPlayers()
{
  GSVe3ViewOtherPlayers* gs = TheGSVe3ViewOtherPlayers::Instance();
  gs->SetPrevState(TheGSVe3HomePage::Instance());
  TheGame::Instance()->SetCurrentState(gs);
}


void GSVe3HomePage::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-ve3-homepage.txt");
  Assert(m_gui);

  GetElementByName(m_gui, "change-name-button")->SetCommand(OnChangeName);
  GetElementByName(m_gui, "change-look-button")->SetCommand(OnChangeLook);
  GetElementByName(m_gui, "explore-button")->SetCommand(OnExplore);
  GetElementByName(m_gui, "other-players-button")->SetCommand(OnOtherPlayers);

  Player* p = GetLocalPlayer();
  Assert(p);

  ShowPlayer(p, m_gui);

/*
  std::string playerName = p->GetName();

  GuiText* name = (GuiText*)GetElementByName(m_gui, "playername-text");
  Assert(name);
  name->SetText(playerName);

  LayerGroups layerGroups; // store settings for each layer
  layerGroups.SetFromSprite(p->GetSprite());
  layerGroups.SetSprite(&m_spriteNode.GetSprite());
*/
}

} // namespace
