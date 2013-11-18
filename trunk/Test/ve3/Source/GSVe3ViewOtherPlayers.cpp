#include <AmjuGL.h>
#include <Timer.h>
#include <Game.h>
#include "GSVe3ViewOtherPlayers.h"
#include "LayerGroup.h"
#include "Player.h"
#include "ObjectManager.h"

namespace Amju
{
GSVe3ViewOtherPlayers::GSVe3ViewOtherPlayers()
{
}

void GSVe3ViewOtherPlayers::Update()
{
  GSGui::Update();

}

void GSVe3ViewOtherPlayers::Draw()
{
  GSGui::Draw();

}

void GSVe3ViewOtherPlayers::Draw2d()
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

static void OnBack()
{
  TheGSVe3ViewOtherPlayers::Instance()->GoBack();
}

static void OnPrevPlayer()
{
  TheGSVe3ViewOtherPlayers::Instance()->PrevPlayer();
}

static void OnNextPlayer()
{
  TheGSVe3ViewOtherPlayers::Instance()->NextPlayer();
}

void GSVe3ViewOtherPlayers::NextPlayer()
{
  GameObjects* gos = TheObjectManager::Instance()->GetGameObjects();
  int id = -1;
  if (m_player)
  {
    id = m_player->GetId();
  }
  for (auto it = gos->begin(); it != gos->end(); ++it)
  {
    GameObject* go = it->second;
    Player* p = dynamic_cast<Player*>(go);

    if (p && p->GetId() > id)
    {
      m_player = p;
      break;
    }
  }

  if (m_player)
  {
    SetPlayerSprite();
  }
}


void GSVe3ViewOtherPlayers::PrevPlayer()
{
  GameObjects* gos = TheObjectManager::Instance()->GetGameObjects();
  int id = -1;
  if (m_player)
  {
    id = m_player->GetId();
  }
  for (auto it = gos->rbegin(); it != gos->rend(); ++it)
  {
    GameObject* go = it->second;
    Player* p = dynamic_cast<Player*>(go);

    if (p && p->GetId() > id)
    {
      m_player = p;
      break;
    }
  }

  if (m_player)
  {
    SetPlayerSprite();
  }
}

void GSVe3ViewOtherPlayers::SetPlayerSprite()
{
  Assert(m_player);
  LayerGroups layerGroups; // store settings for each layer
  layerGroups.SetFromSprite(m_player->GetSprite());
  layerGroups.SetSprite(&m_spriteNode.GetSprite());

  m_spriteNode.Update();

}

void GSVe3ViewOtherPlayers::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-ve3-viewotherplayers.txt");
  Assert(m_gui);

  GetElementByName(m_gui, "back-button")->SetCommand(OnBack);
  GetElementByName(m_gui, "prev-player-button")->SetCommand(OnPrevPlayer);
  GetElementByName(m_gui, "next-player-button")->SetCommand(OnNextPlayer);

  NextPlayer();
}

} // namespace
