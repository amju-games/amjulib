#include "GuiTextEdit.h"
#include <EventPoller.h>
#include <Timer.h>
#include <DrawRect.h>

namespace Amju
{
const char* GuiTextEdit::NAME = "gui-text-edit";

GuiTextEdit::GuiTextEdit() : m_caret(0)
{
  m_just = AMJU_JUST_LEFT;

  m_drawBg = true;
  m_caretTimer = 0;
  m_drawCaret = true;
  m_first = 0;
  m_last = 0;
}

void GuiTextEdit::Draw()
{
  static const float BLINK_TIME_END = 0.5f;
  static const float BLINK_TIME_HALF = BLINK_TIME_END * 0.5f;

  if (!IsVisible())
  {
    return;
  }

  float dt = TheTimer::Instance()->GetDt();
  m_caretTimer += dt;
  m_drawCaret = false;
  if (m_caretTimer > BLINK_TIME_END)
  {
    m_caretTimer = 0;
  }
  else if (HasFocus() && m_caretTimer < BLINK_TIME_HALF)
  {
    m_drawCaret = true;
  }

  // Works best if caret width is zero, (change widths file for char 124)
  if (m_drawCaret)
  {
    std::string left = m_myText.substr(0, m_caret);
    std::string right = m_myText.substr(m_caret);
    m_text = left + "|" + right;
  }
  else
  {
    m_text = m_myText;
  }

  if (HasFocus())
  {
    // Draw border
    // TODO Could be image - allow flexible way to give GUIs themes
    static float t = 0;
    t += dt;
    PushColour();
    float s = (sin(t * 5.0f) + 1.0f) * 0.5f;
    Colour c(s, s, 1, 1);
    AmjuGL::SetColour(c); //inverse ? m_fgCol : m_bgCol);
    AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);

    Rect r = GetRect(this);
    const float BORDER = 0.01f;
    float xmin = r.GetMin(0) - BORDER;
    float xmax = r.GetMax(0) + BORDER;
    float ymin = r.GetMin(1) - BORDER;
    float ymax = r.GetMax(1) + BORDER;
    r.Set(xmin, xmax, ymin, ymax);
    
    DrawSolidRect(r);
    AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
    PopColour();
  }

  GuiText::Draw();
}

bool GuiTextEdit::Load(File* f)
{
  if (!GuiText::Load(f))
  {
    return false;
  }
  m_myText = m_text;
  m_caret = m_text.size();

  return true;
}

const std::string& GuiTextEdit::GetText() const
{
  return m_myText;
}

void GuiTextEdit::SetText(const std::string& text)
{
  m_text = text;
  m_myText = text;
  m_caret = m_text.size();
}

void GuiTextEdit::Insert(char c)
{
  std::string left = m_myText.substr(0, m_caret);
  std::string right = m_myText.substr(m_caret);

  m_myText = left + c + right;
  m_caret++;
}

bool GuiTextEdit::OnCursorEvent(const CursorEvent& ce)
{
  // TODO In drag mode, select part of the text
  return false;
}

bool GuiTextEdit::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  if (!IsVisible())
  {
    return false;
  }

  Rect r = GetRect(this);
  if (!r.IsPointIn(Vec2f(mbe.x, mbe.y)))
  {
    return false; // not handled - cursor not in edit box
  }

  // If clicked in bounding rect, this element gets focus
  SetHasFocus(true);

  if (!mbe.isDown)
  {
    return true;  
  }

  // When L button clicked, find new caret position
  float startX = GetCombinedPos().x;
std::cout << "Mouse X: " << mbe.x << " text start X: " << startX << "\n";
std::cout << "Displayed String: \"" << m_text.substr(m_first, m_last - m_first) << "\"\n";

  for (int i = m_first; i < m_last; i++)
  {
    float x = GetFont()->GetTextWidth(m_text.substr(m_first, i - m_first)) + startX;

std::cout << "String: \"" << m_text.substr(m_first, i - m_first) << "\" X " << x << "\n";

    if (x > mbe.x)
    {
std::cout << "FOUND POS!\n";
      m_caret = i;
      break;
    }
  }
  
  return false;
}

bool GuiTextEdit::OnKeyEvent(const KeyEvent& ke)
{
  if (!IsVisible())
  {
    return false;
  }

  if (!HasFocus())
  {
    return false;
  }

  if (!ke.keyDown)
  {
    return true;
  }

  switch (ke.keyType)
  {
  case AMJU_KEY_CHAR:
    // Prevent chars like Tab 
    if (ke.key >= ' ') // && ke.key <= (char)127)
    {
      Insert(ke.key);
    }
    else
    {
      // TODO Alert user that character is out of range
    }
    break;

  case AMJU_KEY_UP:
  case AMJU_KEY_DOWN:
    // TODO Multi line text
    break;

  case AMJU_KEY_LEFT:
    if (m_caret > 0)
    {
      m_caret--;
    }
    break;

  case AMJU_KEY_RIGHT:
    if (m_caret < (int)m_myText.size())
    {
      m_caret++;
    }
    break;

  case AMJU_KEY_ENTER:
    ExecuteCommand();
    break;

  case AMJU_KEY_SPACE:
    Insert(' ');
    break;

  case AMJU_KEY_ESC:
    // OnCancel();
    break;

  case AMJU_KEY_BACKSPACE:
    if (m_caret > 0)
    {
      std::string left = m_myText.substr(0, m_caret - 1);
      std::string right = m_myText.substr(m_caret);
      m_myText = left + right;
      m_caret--;
    }
    break;

  case AMJU_KEY_DELETE:
    if (m_caret < (int)m_myText.size())
    {
      std::string left = m_myText.substr(0, m_caret);
      std::string right = m_myText.substr(m_caret + 1);
      m_myText = left + right;
    }
    break;

  default:
#ifdef _DEBUG
std::cout << "Unexpected key type: " << ke.keyType << " key: '" << ke.key << "' (int: " << (int)ke.key << ")\n"; 
#endif
//    Assert(0); // handle this key ?
    break;
  }

  return true; // handled
}

void GuiTextEdit::GetFirstLast(int line, int* first, int* last)
{
  *first = 0;
  *last = m_text.size();
  int caret = m_caret;
  if (m_drawCaret)
  {
    caret++;
  }

  Vec2f size = GetSize();  

  switch (m_just)
  {
  case AMJU_JUST_LEFT:
    while (GetFont()->GetTextWidth(m_text.substr(*first, *last - *first)) > size.x)
    {
      if (caret > *last)
      {
        (*last)++;
      }
      else if (caret == *last)
      {
        (*first)++;
      }
      else if (caret < *first)
      {
        (*first)--;
      }
      else
      {
        (*last)--;
      }
    }

    Assert(*last <= (int)m_text.size());
    Assert(*first >= 0);
    Assert(m_caret >= *first);
    Assert(m_caret <= *last);

    m_first = *first;
    m_last = *last;

    return;

  default:
    Assert(0);
  }
}
}
