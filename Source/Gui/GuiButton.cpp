#include <AmjuFirst.h>
#include <Timer.h>
#include <TextToSpeech.h>
#include "CursorManager.h"
#include "GuiButton.h"
#include "Font.h"
#include "Screen.h"
#include "EventPoller.h"
#include "SoundManager.h"
#include "StringUtils.h"
#include <AmjuFinal.h>

#define ENABLE_FOCUS_BUTTON

namespace Amju
{
std::string GuiButton::s_clickFilename;

const char* GuiButton::NAME = "gui-button";

static RCPtr<GuiButton> focusButton = 0;
static RCPtr<GuiButton> cancelButton = 0;

GuiButton::GuiButton()
{
  m_isMouseOver = false;
  m_isPressed = false;
  m_isEnabled = true;
  m_showIfFocus = true;
  m_onPressedDown = 0;
}

GuiButton::~GuiButton()
{
  // Done in Listener dtor TheEventPoller::Instance()->RemoveListener(this); 
}

bool GuiButton::IsEnabled() const
{
  if (!m_onPressedDown && !m_pCommand && !m_commandFunc)
  {
    return false;
  }

  return m_isEnabled;
}

void GuiButton::SetIsEnabled(bool b)
{
  m_isEnabled = b;
}

bool GuiButton::IsCancelButton() const
{
  return (cancelButton == this);
}

void GuiButton::SetIsCancelButton(bool isCancelButton)
{
#ifdef ENABLE_FOCUS_BUTTON

  if (isCancelButton)
  {
    cancelButton = this;
  }
  else
  {
    cancelButton = 0;
  }

#endif // ENABLE_FOCUS_BUTTON
}

bool GuiButton::IsFocusButton() const
{
  return (focusButton == this);
}

void GuiButton::SetIsFocusButton(bool isFocusButton)
{
#ifdef ENABLE_FOCUS_BUTTON

  if (isFocusButton)
  {
    focusButton = this;
  }
  else
  {
    focusButton = 0;
  }

#endif //ENABLE_FOCUS_BUTTON
}

void GuiButton::SetShowIfFocus(bool s)
{
  m_showIfFocus = s;
}

void GuiButton::OnGetFocus()
{
  SetIsFocusButton(true);
}

void GuiButton::TextToSpeech()
{
  if (!m_guiText.GetText().empty())
  {
    Amju::TextToSpeech(m_guiText.GetText());
  }
}

void GuiButton::SetOnPressedDownFunc(CommandFunc f)
{
  m_onPressedDown = f;
}

void GuiButton::OnPressedDown()
{
  if (m_onPressedDown)
  {
    m_onPressedDown();
  }
}

void GuiButton::ClickSound() const
{ 
  if (!s_clickFilename.empty())
  {
    static SoundManager* s = TheSoundManager::Instance();
    s->PlayWav(s_clickFilename);
  }
}

void GuiButton::SetClickFilename(const std::string& clickFilename)
{
  s_clickFilename = clickFilename;
}

bool GuiButton::Load(File* f)
{
  if (!GuiImage::Load(f))
  {
    return false;
  }
  // Button textures don't wrap
  GetTexture()->SetWrapMode(AmjuGL::AMJU_TEXTURE_CLAMP);

  // Load text
  if (!m_guiText.LoadText(f))
  {
    return false;
  }

#ifdef _DEBUG
  // For debugging, set name
  m_guiText.SetName("text for button " + m_name + ": \"" + m_guiText.GetText() + "\"");
#endif

  m_guiText.SetLocalPos(GetLocalPos());
  m_guiText.SetSize(GetSize());
  m_guiText.RecalcFirstLast();
 
  m_guiText.SetParent(GetParent());

  // Check for initial mouse over
  Rect rect = GetRect(this); 
  if (TheCursorManager::Instance()->IsCursorIn(rect))
  {
    SetIsMouseOver(true);
  }
 
  return true;
}

void GuiButton::Draw()
{
  if (!IsVisible())
  {
    return;
  }

  AmjuGL::PushMatrix();
  if (m_isPressed)
  {
    static const float PRESSED_OFFSET = 0.01f;
    AmjuGL::Translate(PRESSED_OFFSET, -PRESSED_OFFSET, 0);
  }

  if (IsMouseOver())
  {
    // TODO This is no good, the size change depends on the position of the button.
    // Should instead rebuild tri list.
    static const float SCALE = 1.01f;
    AmjuGL::Scale(SCALE, SCALE, 1.0f);
  }


  if ((IsFocusButton() || IsCancelButton()) && m_showIfFocus) 
    ///// || HasFocus()) // TODO just one
  {
    // Draw border
    // TODO Could be image - allow flexible way to give GUIs themes
    static float t = 0;
    PushColour();
    if (IsFocusButton())
    {
      float dt = TheTimer::Instance()->GetDt();
      t += dt;
      float s = (sin(t * 5.0f) + 1.0f) * 0.5f;
      Colour c(s, s, 1, 1);
      AmjuGL::SetColour(c); 
    }
    else
    {
      AmjuGL::SetColour(Colour(1, 0, 0, 1));
    }

    Rect r = GetRect(this);
    float BORDER = 0.025f; // TODO configurable
    float xmin = r.GetMin(0) - BORDER;
    float xmax = r.GetMax(0) + BORDER;
    float ymin = r.GetMin(1) - BORDER;
    float ymax = r.GetMax(1) + BORDER;
    r.Set(xmin, xmax, ymin, ymax);
    
    GuiImage focus(*this);
    focus.SetParent(0);
    focus.SetLocalPos(Vec2f(xmin, ymax));
    focus.SetSize(Vec2f(xmax - xmin, ymax - ymin));
    focus.Draw();

    PopColour();
  }

  PushColour();
  AmjuGL::SetColour(m_buttonColour);
  GuiImage::Draw();
  PopColour();

  //PushColour();
  float a = 1.0f;
  if (!IsEnabled())
  {
    a = 0.5f;
  }
  m_guiText.SetLocalPos(GetLocalPos());
  m_guiText.SetFgCol(Colour(m_textColour.m_r, m_textColour.m_g, m_textColour.m_b, a));
  m_guiText.Draw();
  AmjuGL::PushMatrix();
  AmjuGL::Translate(0, 0.01f, 0);
  m_guiText.SetFgCol(Colour(0, 0, 0, a));
  m_guiText.Draw();
  AmjuGL::PopMatrix();

  AmjuGL::PopMatrix();
}

bool GuiButton::OnCursorEvent(const CursorEvent& ce)
{
  if (!IsVisible() ||!IsEnabled())
  {
    return false;
  }

  // TODO Adjust for cursor hot spot - dependency on Cursor Manager ??

  // Point in button rectangle ?
  Rect r = GetRect(this);
  SetIsMouseOver(r.IsPointIn(Vec2f(ce.x, ce.y)));

  // This isn't right - only handled if we click the button
  ////  return m_isMouseOver; // handled if over this button 

  return false;
}

bool GuiButton::OnKeyEvent(const KeyEvent& ke)
{
  if (!IsVisible() ||!IsEnabled())
  {
    return false;
  }

  if ((IsFocusButton()  && ke.keyType == AMJU_KEY_ENTER)  ||
      (IsCancelButton() && ke.keyType == AMJU_KEY_ESC))
  {
    m_isPressed = ke.keyDown;
    if (ke.keyDown)
    {
      ClickSound();
    }
    else
    {
      ExecuteCommand();
    }
    return true; // handled
  }
  return false;
}

bool GuiButton::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  if (!IsVisible() ||!IsEnabled())
  {
    return false;
  }

  // If event is mouse button up, and button is pressed, and mouse is over button,
  // then trigger command.
  if (mbe.button == AMJU_BUTTON_MOUSE_LEFT)
  {
    if (mbe.isDown)
    {
      m_isPressed = IsMouseOver();
      if (m_isPressed)
      {
        ClickSound();
        SetHasFocus(true); // Not sure about this. TODO Make it a flag ?
        OnPressedDown();

        return true; // handled
      }
    }
    else 
    {
      if (IsMouseOver() && m_isPressed) // Only execute if we are on button when we release
      {
        // Execute command for this button
        //ClickSound();
        ExecuteCommand();
        //m_isMouseOver = false; // once clicked, revert to unselected.. OK?? No, is very annoying
        m_isPressed = false; // mouse is released whether command executed or not
        return true; // handled
      }
      m_isPressed = false; // mouse is released whether command executed or not
    }
  }
  return false; // not handled
}

bool GuiButton::OnButtonEvent(const ButtonEvent& be)
{
  if (!IsVisible() ||!IsEnabled())
  {
    return false;
  }

  // TODO Configurable buttons, so static var/manager
  if (be.button == AMJU_BUTTON_A)
  {
    if (be.isDown)
    {
      ClickSound();
      m_isPressed = HasFocus(); 
      return m_isPressed;
    }
    else if (HasFocus())
    {
      // Execute command for this button
      ExecuteCommand();
      m_isPressed = false;
      return true; // handled
    }
  }
  return false; // not handled
}

void GuiButton::SetText(const std::string& text)
{
  //m_text = text;
  m_guiText.SetText(text);
}

void GuiButton::SetTextColour(const Colour& col)
{
  m_textColour = col;
}

void GuiButton::SetButtonColour(const Colour& col)
{
  m_buttonColour = col;
}

bool GuiButton::IsMouseOver() const
{
  return m_isMouseOver;
}

void GuiButton::SetIsMouseOver(bool m)
{
  if (m && !m_isMouseOver)
  {
    // Could vibrate, or TTS the text
  }

  m_isMouseOver = m;

  /* Terrible idea
  if (m)
  {
    SetIsFocusButton(true);
  }
  */
}

bool GuiButton::IsPressed() const
{
  return m_isPressed;
}
}
