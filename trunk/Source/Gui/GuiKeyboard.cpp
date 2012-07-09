#include <EventPoller.h>
#include <DrawRect.h>
#include <AmjuGL.h>
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

GuiKeyboard::GuiKeyboard()
{
  m_page = 0;
}

void GuiKeyboard::ChangePage(int page)
{
  // Hide old page
  GetChild(m_page)->SetVisible(false);

  page = m_page;

  // Show new page
  GetChild(m_page)->SetVisible(true);
}

void GuiKeyboard::Draw()
{
  PushColour();
  static const float G = 0.5f;
  AmjuGL::SetColour(Colour(G, G, G, 1));
  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
  Rect r = GetRect(this);
  DrawSolidRect(r);
  AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
  PopColour();

  GuiWindow::Draw();
}

void GuiKeyboard::SetKbScale(float s)
{
}

bool GuiKeyboard::Load(File* file)
{
  if (!GuiElement::Load(file))
  {
    return false;
  }

  // Get filename for KB layout. This string can be localised, giving language-specific keyboards.

  std::string filename; // = "gui-kb.txt"; // TODO global KB choice, depending on Language setting
  if (!file->GetLocalisedString(&filename))
  {
    file->ReportError("Expected keyboard file name");
    return false;
  }

  File f;
  if (!f.OpenRead(filename))
  {
    return false;
  }

  if (!GuiComposite::Load(&f))
  {
    return false;
  }

  GuiElement* elem = 0;

  // Set command on each key. 
  // TODO this must include all punctuation etc.
  static const char CHARS[] = "abcdefghijklmnopqrstuvwxyz0123456789"; 
  for (unsigned int i = 0; i < 36; i++)
  {
    // We use the member function GetElementByName, which fails without asserting.
    // So chars not in this keyboard are just ignored. 
    elem = GetElementByName(std::string(1, CHARS[i]));
    if (elem)
    {
      KeyEvent ke;
      // Printable char
      ke.keyType = AMJU_KEY_CHAR;
      ke.key = CHARS[i];
      ke.modifier = AMJU_KEY_MOD_NONE;
   
      elem->SetCommand(new KbCommand(ke)); 
    }
  }
  // Special keys - enter, space, mode, delete, shift.
  elem = GetElementByName("space");
  if (elem)
  {
      KeyEvent ke;
      ke.keyType = AMJU_KEY_SPACE;
      elem->SetCommand(new KbCommand(ke)); 
  }  

  elem = GetElementByName("enter");
  if (elem)
  {
      KeyEvent ke;
      ke.keyType = AMJU_KEY_ENTER;
      elem->SetCommand(new KbCommand(ke)); 
  }  

  elem = GetElementByName("del");
  if (elem)
  {
      KeyEvent ke;
      ke.keyType = AMJU_KEY_BACKSPACE;
      elem->SetCommand(new KbCommand(ke)); 
  }  

  // TODO Shift, mode
  // Arrow keys etc ?
  
  GetChild(0)->SetVisible(true);
  for (int i = 1; i < GetNumChildren(); i++)
  {
    GetChild(i)->SetVisible(false);
  }

  return true;
}

}


