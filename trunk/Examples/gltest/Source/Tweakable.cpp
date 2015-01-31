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
  text->SetFontSize(0.5f);
  text->SetFgCol(Colour(1, 1, 1, 1)); 
  text->SetSize(Vec2f(0.5f, 0.05f));
  text->SetJust(GuiText::AMJU_JUST_LEFT);
  text->SetText(label);
  text->SetLocalPos(Vec2f(0.05f, 0));
  AddChild(text);
}

static void OnCheckbox(GuiCheck* check, bool value)
{
  TweakableBool* tweak = (TweakableBool*)check->GetUserData();
  tweak->SetVar(value);
  // TODO Call callback on Tweakable that value has changed..?
}

void Tweakable::AddCheckBox()
{
  GuiCheck* check = new GuiCheck;
  check->SetSize(Vec2f(0.045f, 0.05f));
  check->SetLocalPos(Vec2f(0.3f, 0));
  Texture* checked = (Texture*)TheResourceManager::Instance()->GetRes("checked.png");
  Texture* unchecked = (Texture*)TheResourceManager::Instance()->GetRes("unchecked.png");
  check->Set(checked, unchecked);
  check->SetValue(false);
  check->SetUserData(this);
  check->SetOnChangeValue(OnCheckbox);
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
  m_var = var;
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

