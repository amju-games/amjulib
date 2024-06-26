#include <iostream>

#include <AmjuFirst.h>
#include <AmjuClipboard.h>
#include <EventPoller.h>
#include <Timer.h>
#include <DrawRect.h>
#include "GuiTextEdit.h"
#ifdef ANDROID_NDK
#include <ctype.h> // isalnum
#endif
#include <AmjuFinal.h>

#define TEXT_EDIT_DEBUG

namespace Amju
{
const char* GuiTextEdit::NAME = "gui-text-edit";

GuiTextEdit::GuiTextEdit() 
{
  m_just = AMJU_JUST_LEFT;

  m_drawBg = true;
  m_caretTimer = 0;
  m_drawCaret = true;
  m_isPassword = false;
  m_rect.SetParent(this);
}

void IGuiTextEdit::SetOnChangeFunc(CommandFunc f)
{
  m_onChangeFunc = f;
}

void GuiTextEdit::SetIsPassword(bool isPw)
{
  m_isPassword = isPw;
}

bool GuiTextEdit::IsPassword() const
{
  return m_isPassword;
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
  else if (m_caretTimer < BLINK_TIME_HALF)
  {
    m_drawCaret = true;
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

    const float BORDER = 0.02f;
    m_rect.SetSize(GetSize() + Vec2f(2 * BORDER, 2 * BORDER));
    m_rect.SetLocalPos(Vec2f(-BORDER, BORDER));
    m_rect.SetCornerRadius(BORDER);
    m_rect.SetRoundCorners(0);
    m_rect.SetColour(c);
    m_rect.Draw();

    AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
    PopColour();
  }

  GuiText::Draw();

  if (m_drawCaret && HasFocus())
  {
    // Draw caret - TODO multi line 
    PushColour();
    AmjuGL::SetColour(m_drawCaret ? m_fgCol : m_bgCol);
    float startX = GetCombinedPos().x;
    float x = (GetFont()->GetTextWidth(m_text.substr(m_first, m_caret - m_first)) * 
      GetFontSize() * m_scaleX) + startX;

    if (!m_triListCaret)
    {
      m_triListCaret = GetFont()->MakeTriList(0, 0, "|", 1.0f);
    }

    float y = GetCombinedPos().y - GetFontSize() * CHAR_HEIGHT_FOR_SIZE_1; 
    
    AmjuGL::PushMatrix();
    AmjuGL::Translate(x, y, 0);
    AmjuGL::Draw(m_triListCaret);
    AmjuGL::PopMatrix();
    PopColour();
  }
}

bool GuiTextEdit::Load(File* f)
{
  if (!GuiText::Load(f))
  {
    return false;
  }
// TODO Move to GuiText
  m_caret = m_text.size();
  m_selectedText = m_caret;

  RecalcFirstLast();

  return true;
}

// TODO Move to GuiText
void GuiTextEdit::SetText(const std::string& text)
{
  if (text == m_text)
  {
    return;
  }

  m_triList = 0; // force rebuild
  m_triLists.clear();
  m_triListSelection = 0;

  m_text = text;
  m_caret = m_text.size();
  m_selectedText = m_caret;
  RecalcFirstLast();
}

void GuiTextEdit::Insert(char c)
{
  // If there is a selection, replace the selection with c. Otherwise insert c at m_caret.

  int left = std::min(m_caret, m_selectedText);  
  int right = std::max(m_caret, m_selectedText);  

  std::string leftStr = m_text.substr(0, left);
  std::string rightStr = m_text.substr(right);

  m_text = leftStr + c + rightStr;

  if (left == right)
  {
    m_caret++;
  }
  else
  {
    m_caret = left + 1; // TODO check this
  }
  m_selectedText = m_caret;
  RecalcFirstLast();
  
  if (m_onChangeFunc)
  {
    m_onChangeFunc(this);
  }
  m_triList = 0; // force rebuild
  m_triListSelection = 0;
}

int GuiTextEdit::CalcCursorPos(float mousex)
{
  float startX = GetCombinedPos().x;
  float sizeX = GetSize().x;

  if (mousex < startX)
  {
    return m_first;
  }
  else if (mousex > startX + sizeX)
  {
    return m_last;
  }

  int pos = m_caret;
  for (int i = m_first; i < m_last; i++)
  {
    float x = (GetFont()->GetTextWidth(m_text.substr(m_first, i - m_first)) * 
      GetFontSize() * m_scaleX) + startX;

    if (x > mousex)
    {
      pos = std::max(0, i - 1);
      // DON'T recalc m_first and m_last!
      break;
    }
  }
  return pos;
}

// TODO This should be in GuiText ?
static bool s_drag = false;

bool GuiTextEdit::OnCursorEvent(const CursorEvent& ce)
{
  // TODO In drag mode, select part of the text

  Rect r = GetRect(this);
  if (!r.IsPointIn(Vec2f(ce.x, ce.y)))
  {
    //return false; // not handled - cursor not in edit box
    // We do want to hande this, e.g. if we drag mouse past the right hand end, we want to select all text.
  }

  if (s_drag && HasFocus())
  {
    m_selectedText = CalcCursorPos(ce.x);
    m_triListSelection = 0;
      
#ifdef TEXT_EDIT_DEBUG
std::cout << "Selected: m_caret: " << m_caret << " m_selectedText: " << m_selectedText << ": \"";
if (m_caret > m_selectedText)
{
  std::cout << m_text.substr(m_selectedText, m_caret - m_selectedText);
}
else
{
  std::cout << m_text.substr(m_caret, m_selectedText - m_caret);
}
std::cout << "\"\n";
#endif
  }

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

  // We want to drag to select text even if this is not the focus ?
  //if (HasFocus())
  {
    s_drag = mbe.isDown;
#ifdef TEXT_EDIT_DEBUG
std::cout << "GuiTextEdit Drag is now " << (s_drag ? "ON" : "OFF") << "\n";
#endif
  }

  // If clicked in bounding rect, this element gets focus
  SetHasFocus(true);

  if (!mbe.isDown)
  {
    return true;  
  }

  m_caret = CalcCursorPos(mbe.x);
  m_selectedText = m_caret;
  m_triListSelection = 0;

#ifdef TEXT_EDIT_DEBUG
std::cout << "MB down, resetting selection trilist.\n";
#endif

  return true; // ?
}

static bool IsWordSep(char c)
{
  // TODO Configurable
  if (isalnum(c))
  {
    return false;
  }
  return true;
}

int GuiTextEdit::NextWord(int pos)
{
  do
  {
    pos++;
  }
  while (pos < (int)m_text.size() && !IsWordSep(m_text[pos]));

  if (pos >= (int)m_text.size())
  {
    return (int)m_text.size();
  }

  return pos;
}

int GuiTextEdit::PrevWord(int pos)
{
  if (pos < 1)
  {
    return 0;
  }

  do
  {
    pos--;
  }
  while (pos > 0 && !IsWordSep(m_text[pos])); 
  
  return pos;
}

bool GuiTextEdit::OnKeyEvent(const KeyEvent& ke)
{
  if (!IsVisible())
  {
    return false;
  }

  // Buttons can generate key events, so this GuiTextEdit object may not have the focus (the button
  //  just clicked would have it). So the last GuiTextEdit to have the focus should accept this key.
  if (!HasFocus())
  {
#ifdef TEXT_EDIT_DEBUG
std::cout << "Key event ignored by " << m_name << " as does not have focus.\n";
#endif
    return false;
  }

  // Is this the current or most recent GuiTextEdit to have focus ?
  // TODO 

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
#ifdef MACOSX
    if (ke.modifier & AMJU_KEY_MOD_ALT) // jump to prev word
#else
    if (ke.modifier & AMJU_KEY_MOD_CTRL)
#endif
    {
      // Prev word
      if (ke.modifier & AMJU_KEY_MOD_SHIFT)
      {
#ifdef TEXT_EDIT_DEBUG
std::cout << "Prev word + select\n";
#endif
        m_caret = PrevWord(m_caret);
        m_triListSelection = 0;
      }
      else
      {
#ifdef TEXT_EDIT_DEBUG
std::cout << "Prev word\n";
#endif
        m_caret = PrevWord(m_caret);
        m_selectedText = m_caret; 
        m_triListSelection = 0;
      }
    }
    else if (ke.modifier & AMJU_KEY_MOD_SHIFT)
    {
#ifdef TEXT_EDIT_DEBUG
std::cout << "Prev char + select\n";
#endif
      // Select char to left
      if (m_caret > 0)
      {
        m_caret--;
        m_triListSelection = 0;
      }
    }
    else 
    {
#ifdef TEXT_EDIT_DEBUG
std::cout << "Prev char\n";
#endif
      if (m_caret > 0)
      {
        // Move caret left
        m_caret--;
        m_selectedText = m_caret; 
        m_triListSelection = 0;
      }
    }
    break;

  case AMJU_KEY_RIGHT:
#ifdef MACOSX
    if (ke.modifier & AMJU_KEY_MOD_ALT) // jump to next word
#else
    if (ke.modifier & AMJU_KEY_MOD_CTRL)
#endif
    {
      // Next word
      if (ke.modifier & AMJU_KEY_MOD_SHIFT)
      {
#ifdef TEXT_EDIT_DEBUG
std::cout << "Next word + select\n";
#endif
        m_caret = NextWord(m_caret);
        m_triListSelection = 0;
      }
      else 
      {
#ifdef TEXT_EDIT_DEBUG
std::cout << "Next word\n";
#endif
        m_caret = NextWord(m_caret);
        m_selectedText = m_caret; 
        m_triListSelection = 0;
      }
    }
    else if (ke.modifier & AMJU_KEY_MOD_SHIFT)
    {
#ifdef TEXT_EDIT_DEBUG
std::cout << "Next char + select\n";
#endif
      if (m_caret < (int)m_text.size())
      {
        m_caret++;
        m_triListSelection = 0;
      } 
    }
    else 
    {
#ifdef TEXT_EDIT_DEBUG
std::cout << "Next char\n";
#endif
      if (m_caret < (int)m_text.size())
      {
        m_caret++;
        m_selectedText = m_caret; 
        m_triListSelection = 0;
      }
    }

    break;

  case AMJU_KEY_ENTER:
    // TODO multi-line
    ExecuteCommand();
    break;

  case AMJU_KEY_SPACE:
    Insert(' ');
    break;

  case AMJU_KEY_ESC:
    // OnCancel();
    break;

  case AMJU_KEY_BACKSPACE:
    {
      int left = std::min(m_caret, m_selectedText);  
      int right = std::max(m_caret, m_selectedText);  
      if (left == right && m_caret > 0)
      {
        left--;
      }

      std::string leftStr = m_text.substr(0, left);
      std::string rightStr = m_text.substr(right);
      m_text = leftStr + rightStr;
      m_caret = left;
      m_selectedText = m_caret; 
      m_triListSelection = 0;
  
      if (m_onChangeFunc)
      {
        m_onChangeFunc(this);
      }
      m_triList = 0; // force rebuild
    }
    break;

  case AMJU_KEY_DELETE:
    //if ()
    {
      int left = std::min(m_caret, m_selectedText);  
      int right = std::max(m_caret, m_selectedText);  
      if (left == right && m_caret < (int)m_text.size())
      {
        right++;
      }

      std::string leftStr = m_text.substr(0, left);
      std::string rightStr = m_text.substr(right);
      m_text = leftStr + rightStr;
      m_caret = left;
      m_selectedText = m_caret; 
      m_triListSelection = 0;

      if (m_onChangeFunc)
      {
        m_onChangeFunc(this);
      }
      m_triList = 0; // force rebuild
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
    while ((GetFont()->GetTextWidth(m_text.substr(*first, *last - *first)) * 
      GetFontSize() * m_scaleX) > size.x)
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

bool GuiTextEdit::OnTextEvent(const TextEvent& te)
{
  switch (te.type)
  {
  case AMJU_SELECT_ALL:
    m_caret = m_text.size();
    m_selectedText = 0;
    m_triListSelection = nullptr;
    return true;

  case AMJU_COPY:
    CopyToClipboard(m_text); // TODO Selection
    return true;
  }

  return false;
}
}
