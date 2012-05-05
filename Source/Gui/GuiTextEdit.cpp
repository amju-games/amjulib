#include "GuiTextEdit.h"
#include <EventPoller.h>
#include <Timer.h>
#include <DrawRect.h>

namespace Amju
{
const char* GuiTextEdit::NAME = "gui-text-edit";

GuiTextEdit::GuiTextEdit() 
{
  m_just = AMJU_JUST_LEFT;

  m_drawBg = true;
  m_caretTimer = 0;
  m_drawCaret = true;
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
  if (false) ////m_drawCaret)  // TODO
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

  // Draw caret - TODO multi line 
  PushColour();
  AmjuGL::SetColour(m_drawCaret ? m_fgCol : m_bgCol);
  float startX = GetCombinedPos().x;
  float x = (GetFont()->GetTextWidth(m_myText.substr(m_first, m_caret - m_first)) * GetTextSize()) + startX;
  PrintLine("|", x, GetCombinedPos().y - GetTextSize() * CHAR_HEIGHT_FOR_SIZE_1); // TODO TEMP TEST 
  PopColour();
}

bool GuiTextEdit::Load(File* f)
{
  if (!GuiText::Load(f))
  {
    return false;
  }
  m_myText = m_text;
  m_caret = m_text.size();
  m_selectedText = m_caret;

  RecalcFirstLast();

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
  m_selectedText = m_caret;
  RecalcFirstLast();
}

void GuiTextEdit::Insert(char c)
{
  int left = std::min(m_caret, m_selectedText);  
  int right = std::max(m_caret, m_selectedText);  

  std::string leftStr = m_myText.substr(0, left);
  std::string rightStr = m_myText.substr(right);

  m_myText = leftStr + c + rightStr;

  if (left == right)
  {
    m_caret++;
  }
  else
  {
    m_caret = left + 1;
  }
  m_selectedText = m_caret;
  RecalcFirstLast();
}

int GuiTextEdit::CalcCursorPos(float mousex)
{
  int pos = m_caret;

  float startX = GetCombinedPos().x;

  for (int i = m_first; i < m_last; i++)
  {
    float x = (GetFont()->GetTextWidth(m_myText.substr(m_first, i - m_first)) * GetTextSize()) + startX;

    if (x > mousex)
    {
      pos = std::max(0, i - 1);
      // DON'T recalc m_first and m_last!
      break;
    }
  }
  return pos;
}

static bool s_drag = false;

bool GuiTextEdit::OnCursorEvent(const CursorEvent& ce)
{
  // TODO In drag mode, select part of the text

  Rect r = GetRect(this);
  if (!r.IsPointIn(Vec2f(ce.x, ce.y)))
  {
    return false; // not handled - cursor not in edit box
  }

  if (s_drag && HasFocus())
  {
    m_selectedText = CalcCursorPos(ce.x);
std::cout << "Selected: m_caret: " << m_caret << " m_selectedText: " << m_selectedText << ": \"";
if (m_caret > m_selectedText)
{
  std::cout << m_myText.substr(m_selectedText, m_caret - m_selectedText);
}
else
{
  std::cout << m_myText.substr(m_caret, m_selectedText - m_caret);
}
std::cout << "\"\n";
  }

  return false;
}

bool GuiTextEdit::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  if (!IsVisible())
  {
    return false;
  }

  if (HasFocus())
  {
    s_drag = mbe.isDown;
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

  m_caret = CalcCursorPos(mbe.x);
  m_selectedText = m_caret;

  return false; // ?
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
    if (ke.modifier & AMJU_KEY_MOD_SHIFT)
    {
      if (m_selectedText > 0)
      {
        m_selectedText--;
      }
    }
    else if (m_caret > 0)
    {
      m_caret--;
      m_selectedText = m_caret; // TODO Unless shift is down ?
    }
    break;

  case AMJU_KEY_RIGHT:
    if (ke.modifier & AMJU_KEY_MOD_SHIFT)
    {
      if (m_selectedText < (int)m_myText.size())
      {
        m_selectedText++;
      }
    }
    else if (m_caret < (int)m_myText.size())
    {
      m_caret++;
      m_selectedText = m_caret; // TODO Unless shift is down ?
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
      int left = std::min(m_caret, m_selectedText);  
      int right = std::max(m_caret, m_selectedText);  
      if (left == right)
      {
        left--;
      }

      std::string leftStr = m_myText.substr(0, left);
      std::string rightStr = m_myText.substr(right);
      m_myText = leftStr + rightStr;
      m_caret--;
      m_selectedText = m_caret; 
    }
    break;

  case AMJU_KEY_DELETE:
    if (m_caret < (int)m_myText.size())
    {
      int left = std::min(m_caret, m_selectedText);  
      int right = std::max(m_caret, m_selectedText);  
      if (left == right)
      {
        right++;
      }

      std::string leftStr = m_myText.substr(0, m_caret);
      std::string rightStr = m_myText.substr(m_caret + 1);
      m_myText = leftStr + rightStr;
    }
    break;

  default:
#ifdef _DEBUG
std::cout << "Unexpected key type: " << ke.keyType << " key: '" << ke.key << "' (int: " << (int)ke.key << ")\n"; 
#endif
//    Assert(0); // handle this key ?
    break;
  }

  if (m_caret >= m_last || m_caret <= m_first)
  {
    RecalcFirstLast();
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
    while ((GetFont()->GetTextWidth(m_myText.substr(*first, *last - *first)) * GetTextSize()) > size.x)
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

    if (*last > (int)m_text.size())
    {
      *last = m_text.size();
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
