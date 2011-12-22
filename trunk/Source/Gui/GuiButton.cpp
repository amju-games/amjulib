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

/*
  if (!f->GetDataLine(&m_text))
  {
    f->ReportError("Expected button text");
    return false;
  }
  // Get font name and size
  std::string s;
  if (!f->GetDataLine(&s))
  {
    f->ReportError("Expected font info");
    return false;
  }
  Strings strs = Split(s, ',');
  Assert(strs.size() == 2);
  std::string fontName = "font2d/" + strs[0] + "-font.font";
  m_font = (Font*)TheResourceManager::Instance()->GetRes(fontName);
  m_fontSize = ToFloat(strs[1]);
*/

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
    static const float PRESSED_OFFSET = 0.02f;
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

/*
  // Draw text
  // TODO Get font name, point size - use a GuiText object
  PushColour();
  float w = m_font->GetTextWidth(m_text);
  float x = m_pos.x + m_size.x * 0.5f - w * 0.5f * m_fontSize;
  float y = m_pos.y - m_size.y; // + 0.5f * heighOfChar ??
  float origSize = m_font->GetSize();
  m_font->SetSize(m_fontSize);
  AmjuGL::SetColour(m_textColour.m_r, m_textColour.m_g, m_textColour.m_b, m_textColour.m_a);
  m_font->Print(x, y, m_text.c_str());
  // TODO Flag
  // Drop shadow
  AmjuGL::SetColour(0, 0, 0, 1);
  m_font->Print(x, y + 0.01f, m_text.c_str());

  m_font->SetSize(origSize);
  PopColour();
*/

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
  Rect r = GetRect(this);//(m_pos.x, m_pos.x + m_size.x, m_pos.y - m_size.y, m_pos.y);
  m_isMouseOver = (r.IsPointIn(Vec2f(ce.x, ce.y)));

  // This isn't right - only handled if we click the button
  ////  return m_isMouseOver; // handled if over this button 

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
        m_isMouseOver = false; // once clicked, revert to unselected.. OK??
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
