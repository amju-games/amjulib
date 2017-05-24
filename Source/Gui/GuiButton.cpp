#include <AmjuFirst.h>
#include <AmjuGL.h>
#include <DrawRect.h>
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

//#define BUTTON_DEBUG

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

  // Get the combined colour for this button - disable if less than 50% alpha
  if (m_combinedColour.m_a < 0.5f)
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
#ifdef AMJU_TEXT_TO_SPEECH
  if (!m_guiText.GetText().empty())
  {
    Amju::TextToSpeech(m_guiText.GetText());
  }
#endif
}

void GuiButton::SetOnPressedDownFunc(CommandFunc f)
{
  m_onPressedDown = f;
}

void GuiButton::OnPressedDown()
{
  if (m_onPressedDown)
  {
    m_onPressedDown(this);
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
  m_focusImage.SetTexture(GetTexture());

  // Load text
  if (!m_guiText.LoadText(f))
  {
    return false;
  }

#ifdef _DEBUG
  // For debugging, set name
  m_guiText.SetName("text for button " + m_name + ": \"" + m_guiText.GetText() + "\"");
#endif

  // Button text tends to get truncated, so extend the size of the text
  //  (TODO This might be a bug in RecalcFirstLast when font size != 1)
  Vec2f pos = GetLocalPos();
  Vec2f size = GetSize();
  pos.x -= size.x * 0.25f;
  m_guiText.SetLocalPos(pos);
  size.x *= 1.5f; 
  m_guiText.SetSize(size);
  m_guiText.RecalcFirstLast();
 
  m_guiText.SetParent(GetParent());

  // Use text BG colour to tint the button image, but don't draw a text background, which is a
  //  solid rectangle.
  SetButtonColour(m_guiText.GetBgCol());
  m_guiText.SetDrawBg(false);

#if defined(WIN32) || defined(MACOSX)
  // Check for initial mouse over
  Rect rect = GetRect(this); 
  if (TheCursorManager::Instance()->IsCursorIn(rect))
  {
    SetIsMouseOver(true);
  }
#endif
    
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
    // Scale + Tralsate is required.
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
      Colour c = m_focusColour * Colour(s, s, s, 1);
      MultColour(c);
    }
    else
    {
      MultColour(Colour(1, 0, 0, 1));
    }

    Rect r = GetRect(this);
    float BORDER = 0.025f; // TODO configurable
    float xmin = r.GetMin(0) - BORDER;
    float xmax = r.GetMax(0) + BORDER;
    float ymin = r.GetMin(1) - BORDER;
    float ymax = r.GetMax(1) + BORDER;
    r.Set(xmin, xmax, ymin, ymax);
    
    m_focusImage.SetParent(0);
    m_focusImage.SetLocalPos(Vec2f(xmin, ymax));
    m_focusImage.SetSize(Vec2f(xmax - xmin, ymax - ymin));
    m_focusImage.Draw();
     
//    TODO use m_focusImage - don't create/destroy VBO
    
    PopColour();
  }

  PushColour();
  MultColour(m_buttonColour);
  GuiImage::Draw();
  
  // Get the combined colour for this button - so we can disable in IsEnabled
  m_combinedColour = AmjuGL::GetColour();
  
  PopColour();

  // TODO This would be much simpler if GuiButton was a Composite.
  // Then we could also have layers of images, text, etc.
  // Make sure text is positioned properly, may have been moved in code since Load
  m_guiText.SetParent(GetParent());
  Vec2f pos = GetLocalPos();
  Vec2f size = GetSize();
  pos.x -= size.x * 0.25f;
  m_guiText.SetLocalPos(pos);

  m_guiText.Draw();

#ifdef DRAW_BOUNDING_RECT
  // Draw bounding rect
  AmjuGL::SetIdentity();
  Rect r = GetRect(this);
  PushColour();
  AmjuGL::SetColour(Colour(0, 0, 0, 1));
  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
  DrawRect(r);
  AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
  PopColour();
#endif // DRAW_BOUNDING_RECT

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
        // Don't make button the focus when clicked - this would remove the focus from
        //  any edit box which we want to receive keyboard events.
        //SetHasFocus(true);
        OnPressedDown();
        return true; // handled
      }
    }
    else
    {
#ifdef IPHONE
        if (IsMouseOver())
#else
      if (IsMouseOver() && m_isPressed) // Only execute if we are on button when we release
#endif
      {
        // Execute command for this button
        //ClickSound();
        ExecuteCommand();
        //m_isMouseOver = false; // once clicked, revert to unselected.. OK?? No, is very annoying
        m_isPressed = false; // mouse is released whether command executed or not
#ifdef BUTTON_DEBUG
        std::cout << "Button up for " << m_name << " - executed command\n";
#endif
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
    MouseButtonEvent mbe;
    mbe.isDown = be.isDown;
    mbe.button = AMJU_BUTTON_MOUSE_LEFT;
    OnMouseButtonEvent(mbe);

/*
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
*/
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
  m_guiText.SetFgCol(col);
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

const Colour& GuiButton::GetTextColour() const
{
  return m_guiText.GetFgCol();
}

const Colour& GuiButton::GetButtonColour() const
{
  return m_buttonColour;
}

void GuiButton::SetFocusColour(const Colour& col)
{
  m_focusColour = col;
}

const Colour& GuiButton::GetFocusColour() const
{
  return m_focusColour;
}
}
