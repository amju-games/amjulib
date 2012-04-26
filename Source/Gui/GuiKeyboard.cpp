#include "GuiKeyboard.h"

namespace Amju
{
const char* GuiKeyboard::NAME = "gui-kb";

struct KbCommand : public GuiCommand
{
  KbCommand(char c) : m_char(c)
  {
  }

  virtual bool Do()
  {
    std::cout << "Generating key event: " << m_char << "\n";

    return false; // no undo
  }

  char m_char;
};

bool GuiKeyboard::Load(File*)
{
  const std::string filename = "gui-kb.txt"; // TODO global KB choice 
  File f;
  if (!f.OpenRead(filename))
  {
    Assert(0);
    return false;
  }
  if (! GuiComposite::Load(&f))
  {
    return false;
  }

  // Set command on each key
  static const char CHARS[] = "abcdefghijklmnopqrstuvwxyz0123456789"; 
  for (unsigned int i = 0; i < 36; i++)
  {
//std::cout << "Set command for key " << CHARS[i] << "\n";

    Amju::GetElementByName(this, std::string(1, CHARS[i]))->SetCommand(new KbCommand(CHARS[i])); 
  }
  // Arrow keys etc ?

  return true;
}

}


