#include <Timer.h>
#include <TextToSpeech.h>
#include "GuiButton.h"
#include "Font.h"
#include "Screen.h"
#include "EventPoller.h"
#include "SoundManager.h"
#include "StringUtils.h"

namespace Amju
{
const char* GuiButton::NAME = "gui-button";

static const int BUTTON_PRIORITY = -100; // a pressed button takes priority over most stuff

static GuiButton* focusButton = 0;
static GuiButton* cancelButton = 0;

GuiButton::GuiButton()
{
  TheEventPoller::Instance()->AddListener(this, BUTTON_PRIORITY); 

  m_isMouseOver = false;
  m_isPressed = false;
}

GuiButton::~GuiButton()
{
  // Done in Listener dtor TheEventPoller::Instance()->RemoveListener(this); 
}

bool GuiButton::IsCancelButton() const
{
  return (cancelButton == this);
}

void GuiButton::SetIsCancelButton(bool isCancelButton)
{
  if (isCancelButton)
  {
    cancelButton = this;
  }
  else
  {
    cancelButton = 0;
  }
}

bool GuiButton::IsFocusButton() const
{
  return (focusButton == this);
}

void GuiButton::SetIsFocusButton(bool isFocusButton)
{
  if (isFocusButton)
  {
    focusButton = this;
  }
  else
  {
    focusButton = 0;
  }
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

void GuiButton::ClickSound() const
{ 
  // TODO Get resource from ResourceManager ??
//  TheSoundManager::Instance()->PlayWav("click"); // NB no file ext
}

bool GuiButton::Load(File* f)
{
  if (!GuiImage::Load(f))
  {
    return false;
  }
  // Load text
  if (!m_guiText.LoadText(f))
  {
    return false;
  }
  m_guiText.SetPos(GetPos());
  m_guiText.SetSize(GetSize());

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
/*
  if (m_isMouseOver)
  {
    // TODO This is no good, the size change depends on the position of the button.
    // Should instead rebuild tri list.
    static const float SCALE = 1.01f;
    AmjuGL::Scale(SCALE, SCALE, 1.0f);
  }
*/

  if (IsFocusButton()) ///// || HasFocus()) // TODO just one
  {
    // Draw border
    // TODO Could be image - allow flexible way to give GUIs themes
    static float t = 0;
    float dt = TheTimer::Instance()->GetDt();
    t += dt;
    PushColour();
    float s = (sin(t * 5.0f) + 1.0f) * 0.5f;
    Colour c(s, s, 1, 1);
    AmjuGL::SetColour(c); 

    Rect r = GetRect(this);
    float BORDER = 0.025f; // TODO configurable
    float xmin = r.GetMin(0) - BORDER;
    float xmax = r.GetMax(0) + BORDER;
    float ymin = r.GetMin(1) - BORDER;
    float ymax = r.GetMax(1) + BORDER;
    r.Set(xmin, xmax, ymin, ymax);
    
    GuiImage focus(*this);
    focus.SetPos(Vec2f(xmin, ymax));
    focus.SetSize(Vec2f(xmax - xmin, ymax - ymin));
    focus.Draw();

    PopColour();
  }
  GuiImage::Draw();

  PushColour();
  AmjuGL::SetColour(m_textColour.m_r, m_textColour.m_g, m_textColour.m_b, m_textColour.m_a);
  m_guiText.Draw();
  AmjuGL::PushMatrix();
  AmjuGL::Translate(0, 0.01f, 0);
  AmjuGL::SetColour(0, 0, 0, 1);
  m_guiText.Draw();
  AmjuGL::PopMatrix();
  PopColour();

  AmjuGL::PopMatrix();
}

bool GuiButton::OnCursorEvent(const CursorEvent& ce)
{
  if (!IsVisible())
  {
    return false;
  }

  // TODO Adjust for cursor hot spot - dependency on Cursor Manager ??

  // Point in button rectangle ?
  Rect r = GetRect(this);
  m_isMouseOver = (r.IsPointIn(Vec2f(ce.x, ce.y)));

  // This isn't right - only handled if we click the button
  ////  return m_isMouseOver; // handled if over this button 

  return false;
}

bool GuiButton::OnKeyEvent(const KeyEvent& ke)
{
  if ((IsFocusButton()  && ke.keyType == AMJU_KEY_ENTER)  ||
      (IsCancelButton() && ke.keyType == AMJU_KEY_ESC))
  {
    m_isPressed = ke.keyDown;
    ClickSound();
    if (!ke.keyDown)
    {
      ExecuteCommand();
    }
    return true; // handled
  }
  return false;
}

bool GuiButton::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  if (!IsVisible())
  {
    return false;
  }

  // If event is mouse button up, and button is pressed, and mouse is over button,
  // then trigger command.
  if (mbe.button == AMJU_BUTTON_MOUSE_LEFT)
  {
    if (mbe.isDown)
    {
      m_isPressed = m_isMouseOver;
      if (m_isPressed)
      {
        ClickSound();
        SetHasFocus(true);
        return true; // handled
      }
    }
    else 
    {
      m_isPressed = false; // mouse is released whether command executed or not
      if (m_isMouseOver) // Only execute if we are on button when we release
      {
        // Execute command for this button
        ClickSound();
        ExecuteCommand();
        //m_isMouseOver = false; // once clicked, revert to unselected.. OK?? No, is very annoying
        return true; // handled
      }
    }
  }
  return false; // not handled
}

bool GuiButton::OnButtonEvent(const ButtonEvent& be)
{
  if (!IsVisible())
  {
    return false;
  }

  if (be.button == AMJU_BUTTON_A)
  {
    if (be.isDown)
    {
      m_isPressed = m_isMouseOver;
      return m_isPressed;
    }
    else if (m_isMouseOver)
    {
      // Execute command for this button
      ClickSound();
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

bool GuiButton::IsMouseOver() const
{
  return m_isMouseOver;
}

bool GuiButton::IsPressed() const
{
  return m_isPressed;
}
}
