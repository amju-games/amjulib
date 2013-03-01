#include <AmjuGL.h>
#include <Colour.h>
#include <ResourceManager.h>
#include <ReportError.h>
#include <CursorManager.h>
#include <GuiButton.h>
#include <Timer.h>
#include "GS2dAvatarMod.h"
#include "LocalPlayer.h"

namespace Amju
{
void OnOk()
{
  TheGS2dAvatarMod::Instance()->OnOk();
}

void OnCancel()
{
  TheGS2dAvatarMod::Instance()->OnCancel();
}

void OnNextLayer()
{
  TheGS2dAvatarMod::Instance()->OnNextLayer();
}

void OnPrevLayer()
{
  TheGS2dAvatarMod::Instance()->OnPrevLayer();
}

void OnNextTex()
{
  TheGS2dAvatarMod::Instance()->OnNextTex();
}

void OnPrevTex()
{
  TheGS2dAvatarMod::Instance()->OnPrevTex();
}

void OnNextCol()
{
  TheGS2dAvatarMod::Instance()->OnNextCol();
}

void OnPrevCol()
{
  TheGS2dAvatarMod::Instance()->OnPrevCol();
}


GS2dAvatarMod::GS2dAvatarMod()
{
  // Set up sprite
  std::string tex = "characters/2d/spritesheet.png";
  if (!m_sprite.Load(tex, 16, 1, 0.5f, 1.0f))
  {
    ReportError("FAILED TO LOAD sprite for avatar customisation screen");
    Assert(0);
  }

  // TODO TEMP TEST
  m_sprite.SetCellTime(0.25f);
  m_sprite.SetCellRange(1, 1);
  m_sprite.SetCell(1);
}

void GS2dAvatarMod::Update()
{
  GSGui::Update();

}

void GS2dAvatarMod::Draw()
{
  GSGui::Draw();

}

void GS2dAvatarMod::Draw2d()
{
  //GSGui::Draw2d();
  GSBase::Draw2d();

  /*
  if (m_showLurk)
  {
    TheLurker::Instance()->Draw();
  }
  */

  // Show the current layer as flashing
  static float timer = 0;
  timer += TheTimer::Instance()->GetDt();
  static const float FLASH_TIME = 0.1f;
  int layer = m_layerGroups.GetCurrentLayer();

  if (timer < FLASH_TIME)
  {
    m_sprite.SetLayerVis(layer, false);
  }
  else if (timer < 2 * FLASH_TIME)
  {
    m_sprite.SetLayerVis(layer, true);
  }
  else
  {
    timer = 0;
  }

  m_sprite.DrawLayers(Vec2f(-0.25f, -0.5f), 1.0f);

  if (m_gui)
  {
    m_gui->Draw();
  }

  TheCursorManager::Instance()->Draw();
}

void GS2dAvatarMod::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-avatarmod-2d.txt");
  Assert(m_gui);

  // TODO Set focus element, cancel element, command handlers
  GuiButton* nextLayer = (GuiButton*)GetElementByName(m_gui, "layer-next-button");
  Assert(nextLayer);
  nextLayer->SetCommand(Amju::OnNextLayer);

  GuiButton* prevLayer = (GuiButton*)GetElementByName(m_gui, "layer-prev-button");
  Assert(prevLayer);
  prevLayer->SetCommand(Amju::OnPrevLayer);

  GuiButton* nextTex = (GuiButton*)GetElementByName(m_gui, "tex-next-button");
  Assert(nextTex);
  nextTex->SetCommand(Amju::OnNextTex);

  GuiButton* prevTex = (GuiButton*)GetElementByName(m_gui, "tex-prev-button");
  Assert(prevTex);
  prevTex->SetCommand(Amju::OnPrevTex);

  GuiButton* nextCol = (GuiButton*)GetElementByName(m_gui, "col-next-button");
  Assert(nextCol);
  nextCol->SetCommand(Amju::OnNextCol);

  GuiButton* prevCol = (GuiButton*)GetElementByName(m_gui, "col-prev-button");
  Assert(prevCol);
  prevCol->SetCommand(Amju::OnPrevCol);

  GuiButton* ok = (GuiButton*)GetElementByName(m_gui, "ok-button");
  Assert(ok);
  ok->SetCommand(Amju::OnOk);

  GuiButton* cancel = (GuiButton*)GetElementByName(m_gui, "cancel-button");
  Assert(cancel);
  cancel->SetCommand(Amju::OnCancel);

  // TODO Set layer groups from player
  m_layerGroups.SetFromSprite(GetLocalPlayer()->GetSprite());
  m_layerGroups.SetSprite(&m_sprite);
}

void GS2dAvatarMod::OnNextLayer()
{
  m_sprite.SetLayerVis(m_layerGroups.GetCurrentLayer(), true);
  m_layerGroups.NextLayer(); 
}

void GS2dAvatarMod::OnPrevLayer()
{
  m_sprite.SetLayerVis(m_layerGroups.GetCurrentLayer(), true);
  m_layerGroups.PrevLayer(); 
}

void GS2dAvatarMod::OnNextTex()
{
  m_sprite.SetLayerVis(m_layerGroups.GetCurrentLayer(), true);
  m_layerGroups.NextTexture();
  m_layerGroups.SetSprite(&m_sprite);
}

void GS2dAvatarMod::OnPrevTex()
{
  m_sprite.SetLayerVis(m_layerGroups.GetCurrentLayer(), true);
  m_layerGroups.PrevTexture();
  m_layerGroups.SetSprite(&m_sprite);
}

void GS2dAvatarMod::OnNextCol()
{
  m_sprite.SetLayerVis(m_layerGroups.GetCurrentLayer(), true);
  m_layerGroups.NextColour();
  m_layerGroups.SetSprite(&m_sprite);
}

void GS2dAvatarMod::OnPrevCol()
{
  m_sprite.SetLayerVis(m_layerGroups.GetCurrentLayer(), true);
  m_layerGroups.PrevColour();
  m_layerGroups.SetSprite(&m_sprite);
}

void GS2dAvatarMod::OnOk()
{
  m_layerGroups.SendToServer();
  m_layerGroups.SetSprite(&(GetLocalPlayer()->GetSprite()));
  GoBack();
}

void GS2dAvatarMod::OnCancel()
{
  GoBack();
}
} // namespace
