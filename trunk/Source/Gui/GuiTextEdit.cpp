#include "GuiTextEdit.h"
#include <EventPoller.h>

namespace Amju
{
const char* GuiTextEdit::NAME = "gui-text-edit";

GuiTextEdit::GuiTextEdit() : m_caret(0)
{
  m_drawBg = true;

  // Obviously wrong :-(
  TheEventPoller::Instance()->AddListener(this); 
}

void GuiTextEdit::Draw()
{
  std::string left = m_myText.substr(0, m_caret);
  std::string right = m_myText.substr(m_caret);
  m_text = left + "|" + right;

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

std::string GuiTextEdit::GetText() const
{
  return m_myText;
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
    m_myText += ke.key;
    m_caret++;
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
    m_text += " ";
    m_caret++;
    break;

  case AMJU_KEY_ESC:
    // OnCancel();
    break;

  case AMJU_KEY_BACKSPACE:

  case AMJU_KEY_DELETE:
    break;

  }

  return true; // handled
}
}
