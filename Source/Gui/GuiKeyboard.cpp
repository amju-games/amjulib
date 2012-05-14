#include <EventPoller.h>
#include "GuiKeyboard.h"

namespace Amju
{
const char* GuiKeyboard::NAME = "gui-kb";

struct KbCommand : public GuiCommand
{
  KbCommand(const KeyEvent& ke) : m_ke(ke)
  {
  }

  virtual bool Do()
  {
    // Generate key down followed by key up event
    KeyEvent* k1 = new KeyEvent(m_ke);
    k1->keyDown = true;
    TheEventPoller::Instance()->GetImpl()->QueueEvent(k1);

    KeyEvent* k2 = new KeyEvent(m_ke);
    k2->keyDown = false;
    TheEventPoller::Instance()->GetImpl()->QueueEvent(k2);
 
    return false; // no undo
  }

  KeyEvent m_ke;
};

bool GuiKeyboard::Load(File* file)
{
  if (!GuiElement::Load(file))
  {
    return false;
  }

  const std::string filename = "gui-kb.txt"; // TODO global KB choice, depending on Language setting
  File f;
  if (!f.OpenRead(filename))
  {
    Assert(0);
    return false;
  }

  if (!GuiComposite::Load(&f))
  {
    return false;
  }

  // Set command on each key
  static const char CHARS[] = "abcdefghijklmnopqrstuvwxyz0123456789"; 
  for (unsigned int i = 0; i < 36; i++)
  {
//std::cout << "Set command for key " << CHARS[i] << "\n";

    KeyEvent ke;
    // Printable char
    ke.keyType = AMJU_KEY_CHAR;
    ke.key = CHARS[i];
    ke.keyDown = true;
    ke.modifier = AMJU_KEY_MOD_NONE;
    
    Amju::GetElementByName(this, std::string(1, CHARS[i]))->SetCommand(new KbCommand(ke)); 
  }
  // Arrow keys etc ?

  return true;
}

}


