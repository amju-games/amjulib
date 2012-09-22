#include "GSGuiTest.h"
#include <iostream>
#include <AmjuGL.h>
#include <GuiListBox.h>
#include <GuiFileListBox.h>

namespace Amju
{
GSGuiTest::GSGuiTest()
{
}

void GSGuiTest::Update()
{
  GSGui::Update();

}

void GSGuiTest::Draw()
{
  AmjuGL::SetClearColour(Colour(0, 0, 1, 1));
  GSGui::Draw();
}

void GSGuiTest::Draw2d()
{
  if (m_menu)
  {
    m_menu->Draw();
  }

  GSGui::Draw2d(); // Draw cursor last
}

void OnOK()
{
  std::cout << "OK Button pressed.\n";
}

void GSGuiTest::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-test.txt");
  Assert(m_gui);
  
  GuiFileListBox* fb = dynamic_cast<GuiFileListBox*>(GetElementByName(m_gui, "fb"));
  Assert(fb);

  // Set default/start dir
#ifdef WIN32
  fb->SetDir("c:\\");
#endif

#ifdef MACOSX
  fb->SetDir("/");
#endif

/*
  GetElementByName(m_gui, "ok-button")->SetCommand(OnOK);
  GetElementByName(m_gui, "edit1")->SetHasFocus(true);

  GuiListBox* listbox = dynamic_cast<GuiListBox*>(GetElementByName(m_gui, "listbox"));
  Assert(listbox);
  for (int i = 0; i < 10; i++)
  {
    GuiText* t = new GuiText;
    t->SetText("List item " + ToString(i));
   
    listbox->AddItem(t);
  }
*/
}

bool GSGuiTest::OnCursorEvent(const CursorEvent& ce)
{
  return false;
}

void OnChoice()
{
  std::cout << "Menu choice selected!\n";
}

bool GSGuiTest::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  if (mbe.button == AMJU_BUTTON_MOUSE_RIGHT && mbe.isDown)
  {
    m_menu = new GuiMenu;
    m_menu->SetLocalPos(Vec2f(mbe.x, mbe.y));
    
    m_menu->AddChild(new GuiMenuItem("Choice one", OnChoice));
    m_menu->AddChild(new GuiMenuItem("Choice two", OnChoice));

    GuiMenu* childMenu = new GuiMenu;
    childMenu->SetName("Child menu");

    // Get types from Game Object Factory..?
    childMenu->AddChild(new GuiMenuItem("Choice three", OnChoice));
    // TODO Other types

    m_menu->AddChild(new GuiNestMenuItem("Nested menu >", childMenu));
  }

  return false;
}
} // namespace
