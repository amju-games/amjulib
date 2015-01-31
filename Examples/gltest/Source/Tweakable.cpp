#include <GuiCheck.h>
#include <GuiText.h>
#include <GuiTextEdit.h>
#include "Tweakable.h"

namespace Amju
{
static const float VERTICAL_SPACE = 0.02f;

void AddTweakable(GuiDialog* dialog, Tweakable* tweak)
{
  // TODO Position
  float y = -VERTICAL_SPACE; // ?
  int i = dialog->GetNumChildren();
  if (i > 0)
  {
    GuiElement* child = dialog->GetChild(i - 1);
    y = child->GetLocalPos().y - child->GetSize().y - VERTICAL_SPACE * 2.0f;
  }
  
  Vec2f pos = tweak->GetLocalPos();
  pos.y = y;
  tweak->SetLocalPos(pos);

  dialog->AddChild(tweak);
}

void Tweakable::SetLabel(const std::string& label)
{
  GuiText* text = new GuiText;
  //text->SetFontSize(0.5f);
  text->SetSize(Vec2f(1.0f, 0.1f));
  text->SetJust(GuiText::AMJU_JUST_LEFT);
  text->SetText(label);
  text->SetLocalPos(Vec2f(0, 0));
  AddChild(text);
}

void Tweakable::AddCheckBox()
{
  GuiCheck* check = new GuiCheck;
  check->SetSize(Vec2f(0.1f, 0.1f));
  check->SetLocalPos(Vec2f(0.3, 0));
  Texture* checked = (Texture*)TheResourceManager::Instance()->GetRes("checked.png");
  Texture* unchecked = (Texture*)TheResourceManager::Instance()->GetRes("unchecked.png");
  check->Set(checked, unchecked);
  AddChild(check);
}

void Tweakable::AddEditBox()
{
  GuiTextEdit* text = new GuiTextEdit;
  //text->SetFontSize(0.5f);
  text->SetSize(Vec2f(0.3f, 0.1f));
  text->SetJust(GuiText::AMJU_JUST_LEFT);
  text->SetText("value");
  text->SetLocalPos(Vec2f(0.3, 0));
  AddChild(text);
}

TweakableBool::TweakableBool(const std::string& label, bool* var)
{
  SetLabel(label);
  AddCheckBox();
  SetSizeFromChildren();
}

TweakableFloat::TweakableFloat(const std::string& label, float* var, float min, float max) :
  m_var(var), m_min(min), m_max(max)
{
  SetLabel(label);

  // Add edit box for text value
  AddEditBox();

  SetSizeFromChildren();
}
}

