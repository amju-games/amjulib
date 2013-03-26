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

static const float SCALE = 0.1f; // scale increments

void OnShorter()
{
  TheGS2dAvatarMod::Instance()->OnScale(Vec2f(0, -SCALE));
}

void OnTaller()
{
  TheGS2dAvatarMod::Instance()->OnScale(Vec2f(0, +SCALE));
}

void OnThinner()
{
  TheGS2dAvatarMod::Instance()->OnScale(Vec2f(-SCALE, 0));
}

void OnFatter()
{
  TheGS2dAvatarMod::Instance()->OnScale(Vec2f(+SCALE, 0));
}


class ColourCommand : public GuiCommand
{
public:
  ColourCommand(int colour) : m_colour(colour) {}

  virtual bool Do()
  {
    TheGS2dAvatarMod::Instance()->OnSetColour(m_colour);
    return false;
  }

private:
  int m_colour;
};

class LayerCommand : public GuiCommand
{
public:
  LayerCommand(int i, GuiElement* group) : m_layer(i), m_group(group) {}

  virtual bool Do()
  {
    TheGS2dAvatarMod::Instance()->ActivateGroup(m_group);
    TheGS2dAvatarMod::Instance()->OnSetLayer(m_layer);

    return false;
  }

private:
  int m_layer;
  GuiElement* m_group;
};

class TextureCommand : public GuiCommand
{
public:
  TextureCommand(int tex) : m_tex(tex) {}

  virtual bool Do()
  {
    TheGS2dAvatarMod::Instance()->OnSetTexture(m_tex);
    return false;
  }

private:
  int m_tex;
};

GS2dAvatarMod::GS2dAvatarMod()
{
  /*
  // Set up sprite
  std::string tex = "characters/2d/spritesheet.png";
  if (!m_sprite.Load(tex, 16, 1, 0.5f, 1.0f))
  {
    ReportError("FAILED TO LOAD sprite for avatar customisation screen");
    Assert(0);
  }

  // Sprite cell to display
  m_sprite.SetCellTime(0.25f);
  m_sprite.SetCellRange(1, 1);
  m_sprite.SetCell(1);
  */

  m_oldGroup = 0;
}

void GS2dAvatarMod::Update()
{
  GSGui::Update();
  m_spriteNode.Update();
}

void GS2dAvatarMod::Draw()
{
  GSGui::Draw();

}

void GS2dAvatarMod::Draw2d()
{
  GSBase::Draw2d();
  // Not: GSGui::Draw2d();

  // Show the current layer as flashing
  /*
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
  */

  //m_sprite.DrawLayers(Vec2f(-0.25f, -0.5f), 1.0f);
  AmjuGL::PushMatrix();
  AmjuGL::RotateX(90.0f); 
  static float f = 0;
  f += TheTimer::Instance()->GetDt();
  float s = sin(f) * 0.001f;
  AmjuGL::Scale(0.01f, 1, 0.01f);
  AmjuGL::Translate(0, 0, 0.2f);
  AmjuGL::Scale(1, 1, 1.0f + s);

  m_spriteNode.Draw();
  AmjuGL::PopMatrix();

  if (m_gui)
  {
    m_gui->Draw();
  }

  TheCursorManager::Instance()->Draw();
}

void GS2dAvatarMod::ActivateGroup(GuiElement* newGroup)
{
  if (m_oldGroup)
  {
    m_oldGroup->SetVisible(false);
  }

  if (newGroup)
  {
    newGroup->SetVisible(true);
  }

  m_oldGroup = newGroup;
}

void GS2dAvatarMod::OnActive()
{
  GSGui::OnActive();

  if (!m_bgImage.OpenAndLoad("bgimage-avatar-mod.txt"))
  {
std::cout << "Failed to load GUI bg image for avatar mod screen!\n";
    Assert(0);
  }

  m_gui = LoadGui("gui-avatarmod-2d.txt");
  Assert(m_gui);

  GuiButton* ok = (GuiButton*)GetElementByName(m_gui, "ok-button");
  Assert(ok);
  ok->SetCommand(Amju::OnOk);

  GuiButton* cancel = (GuiButton*)GetElementByName(m_gui, "cancel-button");
  Assert(cancel);
  cancel->SetCommand(Amju::OnCancel);

  LayerGroupManager* lgm = TheLayerGroupManager::Instance();
  std::string GROUPNAME[] = 
  {
    "head-body-group", "eyes-group", "hair-group", "bottoms-group", "tops-group"
  };

  // Layer buttons
  for (int i = 0; i < 5; i++)
  {
    GuiButton* b = (GuiButton*)GetElementByName(m_gui, "layer-button-" + ToString(i));
    Assert(b);

    // Find group of buttons to activate when layer button selected
    GuiElement* group = 0;
    if (!GROUPNAME[i].empty())
    {
      group = GetElementByName(m_gui, GROUPNAME[i]);
      Assert(group);
    }
    b->SetCommand(new LayerCommand(i, group));
  }

  // Set the buttons for the different layers
  ((GuiButton*)GetElementByName(m_gui, "hair-button-1"))->SetCommand(new TextureCommand(0));
  ((GuiButton*)GetElementByName(m_gui, "hair-button-2"))->SetCommand(new TextureCommand(1));
  ((GuiButton*)GetElementByName(m_gui, "hair-button-3"))->SetCommand(new TextureCommand(2));

  ((GuiButton*)GetElementByName(m_gui, "scale-button-1"))->SetCommand(OnShorter);
  ((GuiButton*)GetElementByName(m_gui, "scale-button-2"))->SetCommand(OnTaller);
  ((GuiButton*)GetElementByName(m_gui, "scale-button-3"))->SetCommand(OnThinner);
  ((GuiButton*)GetElementByName(m_gui, "scale-button-4"))->SetCommand(OnFatter);

  // Set colour buttons
  for (int i = 0; i < 16; i++)
  {
    GuiButton* b = (GuiButton*)GetElementByName(m_gui, "colour-button-" + 
      ToString(i + 1));
    Assert(b);

    Colour c = lgm->GetColour(i);
    b->SetButtonColour(c); 
    b->SetCommand(new ColourCommand(i));
  }
 
  // Set layer groups from player
  m_layerGroups.SetFromSprite(GetLocalPlayer()->GetSprite());
  m_layerGroups.SetSprite(&m_spriteNode.GetSprite());
}

void GS2dAvatarMod::OnSetTexture(int texture)
{
//  m_sprite.SetLayerVis(m_layerGroups.GetCurrentLayer(), true);

  m_layerGroups.SetTexture(texture);
//  m_layerGroups.SetSprite(&m_sprite);
  m_layerGroups.SetSprite(&m_spriteNode.GetSprite());
}

void GS2dAvatarMod::OnSetColour(int colour)
{
 // m_sprite.SetLayerVis(m_layerGroups.GetCurrentLayer(), true);

  m_layerGroups.SetColour(colour);
  //m_layerGroups.SetSprite(&m_sprite);
  m_layerGroups.SetSprite(&m_spriteNode.GetSprite());
}

void GS2dAvatarMod::OnSetLayer(int layer)
{
  //m_sprite.SetLayerVis(m_layerGroups.GetCurrentLayer(), true);

  m_layerGroups.SetCurrentLayer(layer);
//  m_layerGroups.SetSprite(&m_sprite);
  m_layerGroups.SetSprite(&m_spriteNode.GetSprite());

}

void GS2dAvatarMod::OnScale(const Vec2f& scale)
{
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
