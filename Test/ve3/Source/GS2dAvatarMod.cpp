#include <AmjuFirst.h>
#include <AmjuGL.h>
#include <Colour.h>
#include <ResourceManager.h>
#include <ReportError.h>
#include <CursorManager.h>
#include <GuiButton.h>
#include <Timer.h>
#include <Game.h>
#include "GS2dAvatarMod.h"
#include "LocalPlayer.h"
#include "Useful.h"
#include "ObjectUpdater.h"
#include "ROConfig.h"
#include "GSVe3Mug.h"
#include <AmjuFinal.h>

namespace Amju
{
static void OnOk(GuiElement*)
{
  TheGS2dAvatarMod::Instance()->OnOk();
}

static void OnCancel(GuiElement*)
{
  TheGS2dAvatarMod::Instance()->OnCancel();
}

static const float SCALE = 0.1f; // scale increments

void OnShorter(GuiElement*)
{
  TheGS2dAvatarMod::Instance()->OnScale(Vec2f(0, -SCALE));
}

void OnTaller(GuiElement*)
{
  TheGS2dAvatarMod::Instance()->OnScale(Vec2f(0, +SCALE));
}

void OnThinner(GuiElement*)
{
  TheGS2dAvatarMod::Instance()->OnScale(Vec2f(-SCALE, 0));
}

void OnFatter(GuiElement*)
{
  TheGS2dAvatarMod::Instance()->OnScale(Vec2f(+SCALE, 0));
}

void OnBlank(GuiElement*)
{
  TheGS2dAvatarMod::Instance()->OnBlank();
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
  RCPtr<GuiElement> m_group;
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
  m_oldGroup = 0;
  m_scale = Vec2f(1.0f, 1.0f);
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

  AmjuGL::PushMatrix();
  // Scale for 'breathing' effect..?
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

template<class FUNC>
void GS2dAvatarMod::SetButtonCommand(const std::string& buttonName, FUNC func)
{
  GuiButton* b = ((GuiButton*)GetElementByName(m_gui, buttonName));
  Assert(b);
  b->GetTexture()->SetFilter(AmjuGL::AMJU_TEXTURE_NEAREST);
  b->SetCommand(func);
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
  static const int NUM_GROUP_NAMES = 7;
  std::string GROUPNAME[NUM_GROUP_NAMES] = 
  {
    "head-body-group", "hair-group", "bottoms-group", "tops-group", "glasses-group", "mouth-group", "facial-group"
  };

  // Layer buttons
  for (int i = 0; i < NUM_GROUP_NAMES; i++)
  {
    GuiButton* b = (GuiButton*)GetElementByName(m_gui, "layer-button-" + ToString(i));
    Assert(b);

    // Find group of buttons to activate when layer button selected
    GuiElement* group = 0;
    if (!GROUPNAME[i].empty())
    {
      group = GetElementByName(m_gui, GROUPNAME[i]);
      Assert(group);
      group->SetVisible(false);
    }
    b->SetCommand(new LayerCommand(i, group));
  }

  // Set the buttons for the different layers
  SetButtonCommand("hair-button-1", new TextureCommand(0));
  SetButtonCommand("hair-button-2", new TextureCommand(1));
  SetButtonCommand("hair-button-3", new TextureCommand(2)); //Amju::OnBlank); // invis
  SetButtonCommand("hair-button-4", new TextureCommand(3));

  SetButtonCommand("scale-button-1", OnShorter);
  SetButtonCommand("scale-button-2", OnTaller);
  SetButtonCommand("scale-button-3", OnThinner);
  SetButtonCommand("scale-button-4", OnFatter);

  SetButtonCommand("tops-button-1", new TextureCommand(0));
  SetButtonCommand("tops-button-2", new TextureCommand(1));
  SetButtonCommand("tops-button-3", new TextureCommand(2)); //Amju::OnBlank); // invis

  SetButtonCommand("bots-button-1", new TextureCommand(0));
  SetButtonCommand("bots-button-2", new TextureCommand(1));
  SetButtonCommand("bots-button-3", new TextureCommand(2)); //Amju::OnBlank); // invis

  SetButtonCommand("glasses-button-1", new TextureCommand(0));
  SetButtonCommand("glasses-button-2", new TextureCommand(1)); // invis

  SetButtonCommand("facial-button-1", new TextureCommand(0));
  SetButtonCommand("facial-button-2", new TextureCommand(1));
  SetButtonCommand("facial-button-3", new TextureCommand(2)); //Amju::OnBlank); // invis

  // Set colour buttons
  for (int i = 0; i < 16; i++)
  {
    GuiButton* b = (GuiButton*)GetElementByName(m_gui, "colour-button-" + 
      ToString(i + 1));
    Assert(b);

    b->GetTexture()->SetFilter(AmjuGL::AMJU_TEXTURE_NEAREST);

    Colour c = lgm->GetColour(i);
    b->SetButtonColour(c); 
    b->SetCommand(new ColourCommand(i));
  }
 
  // Set layer groups from player
  m_layerGroups.SetFromSprite(GetLocalPlayer()->GetSprite());
  m_layerGroups.SetSprite(&m_spriteNode.GetSprite());
  // Set scale from player
  m_scale = GetLocalPlayer()->GetScale(); 
  m_spriteNode.SetScale(m_scale);
}

void GS2dAvatarMod::OnSetTexture(int texture)
{
  m_layerGroups.SetTexture(texture);
  m_layerGroups.SetSprite(&m_spriteNode.GetSprite());
}

void GS2dAvatarMod::OnSetColour(int colour)
{
  m_layerGroups.SetColour(colour);
  m_layerGroups.SetSprite(&m_spriteNode.GetSprite());
}

void GS2dAvatarMod::OnSetLayer(int layer)
{
  m_layerGroups.SetCurrentLayer(layer);
  m_layerGroups.SetSprite(&m_spriteNode.GetSprite());
}

void GS2dAvatarMod::OnScale(const Vec2f& scale)
{
  static const float SCALE_MIN_X = ROConfig()->GetFloat("scale-min-x", 0.5f);
  static const float SCALE_MAX_X = ROConfig()->GetFloat("scale-max-x", 1.5f);
  static const float SCALE_MIN_Y = ROConfig()->GetFloat("scale-min-y", 0.5f);
  static const float SCALE_MAX_Y = ROConfig()->GetFloat("scale-max-y", 1.5f);

  m_scale += scale;

  if (m_scale.x < SCALE_MIN_X)
  {
    m_scale.x = SCALE_MIN_X;
    // TODO Beep
  }
  if (m_scale.x > SCALE_MAX_X)
  {
    m_scale.x = SCALE_MAX_X; 
  }
  if (m_scale.y < SCALE_MIN_Y)
  {
    m_scale.y = SCALE_MIN_Y;
    // TODO Beep
  }
  if (m_scale.y > SCALE_MAX_Y)
  {
    m_scale.y = SCALE_MAX_Y;  
  }

  m_spriteNode.SetScale(m_scale);
}

void GS2dAvatarMod::OnBlank()
{
  // TODO
}

void GS2dAvatarMod::OnOk()
{
  // Send scale
  int id = GetLocalPlayerId();
  std::string key = "scale_sprite";
  std::string val = ToString(m_scale);
  TheObjectUpdater::Instance()->SendUpdateReq(id, key, val);

  m_layerGroups.SendToServer();
  m_layerGroups.SetSprite(&(GetLocalPlayer()->GetSprite()));

  // Set scale in local player
  GetLocalPlayer()->SetScale(m_scale);

//  GoBack();
  GSVe3Mug* gsmug = TheGSVe3Mug::Instance();
  gsmug->SetPrevState(GetPrevState());
  TheGame::Instance()->SetCurrentState(gsmug);
}

void GS2dAvatarMod::OnCancel()
{
  GoBack();
}
} // namespace
