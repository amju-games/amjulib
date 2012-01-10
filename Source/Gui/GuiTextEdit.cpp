#include "GuiTextEdit.h"
#include <EventPoller.h>
#include <Timer.h>

namespace Amju
{
const char* GuiTextEdit::NAME = "gui-text-edit";

GuiTextEdit::GuiTextEdit() : m_caret(0)
{
  m_just = AMJU_JUST_LEFT;

  m_drawBg = true;
  m_caretTimer = 0;
  m_drawCaret = true;

  // Obviously wrong :-(
  TheEventPoller::Instance()->AddListener(this); 
}

void GuiTextEdit::Draw()
{
  static const float BLINK_TIME_END = 0.5f;
  static const float BLINK_TIME_HALF = BLINK_TIME_END * 0.5f;

  m_caretTimer += TheTimer::Instance()->GetDt();
  m_drawCaret = true;
  if (m_caretTimer > BLINK_TIME_END)
  {
    m_caretTimer = 0;
  }
  else if (m_caretTimer > BLINK_TIME_HALF)
  {
    m_drawCaret = false;
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
  m_myText = text;
}

void GuiTextEdit::Insert(char c)
{
  std::string left = m_myText.substr(0, m_caret);
  std::string right = m_myText.substr(m_caret);

  m_myText = left + c + right;
  m_caret++;
}

bool GuiTextEdit::OnKeyEvent(const KeyEvent& ke)
{
  if (!ke.keyDown)
  {
    return true;
  }

  switch (ke.keyType)
  {
  case AMJU_KEY_CHAR:
    Insert(ke.key);
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
std::cout << "Unexpected key type: " << ke.keyType << " key: '" << ke.key << "' (int: " << (int)ke.key << ")\n"; 
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

  switch (m_just)
  {
  case AMJU_JUST_LEFT:
    while (GetFont()->GetTextWidth(m_text.substr(*first, *last - *first)) > m_size.x)
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

    return;

  default:
    Assert(0);
  }
}
}
