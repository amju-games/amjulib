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

namespace Amju
{
GSVe3HomePage::GSVe3HomePage()
{
}

void GSVe3HomePage::Update()
{
  GSGui::Update();

  Player* p = GetLocalPlayer();
  Assert(p);
  LayerGroups layerGroups; // store settings for each layer
  layerGroups.SetFromSprite(p->GetSprite());
  layerGroups.SetSprite(&m_spriteNode.GetSprite());

  m_spriteNode.Update();

  // TODO TEMP TEST - Absolutely DON'T keep polling the server!!!!!!
  TheObjectManager::Instance()->Update();

  TheObjectUpdater::Instance()->Update();

}

void GSVe3HomePage::Draw()
{
  GSGui::Draw();

}

void GSVe3HomePage::Draw2d()
{
  GSGui::Draw2d();

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
  TheGame::Instance()->SetCurrentState(gs);
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


}

} // namespace
