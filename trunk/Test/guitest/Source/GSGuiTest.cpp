#include "GSGuiTest.h"
#include <iostream>
#include <AmjuGL.h>
#include <GuiListBox.h>

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

  m_gui->GetElementByName("ok-button")->SetCommand(OnOK);
  GuiListBox* listbox = dynamic_cast<GuiListBox*>(m_gui->GetElementByName("listbox"));
  Assert(listbox);
  for (int i = 0; i < 10; i++)
  {
    GuiText* t = new GuiText;
    t->SetText("List item " + ToString(i));
   
    listbox->AddItem(t);
  }
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
    m_menu->SetPos(Vec2f(mbe.x, mbe.y));
    
    m_menu->AddItem(new GuiMenuItem("Choice one", OnChoice));
    m_menu->AddItem(new GuiMenuItem("Choice two", OnChoice));

    GuiMenu* childMenu = new GuiMenu;
    childMenu->SetName("Child menu");

    // Get types from Game Object Factory..?
    childMenu->AddItem(new GuiMenuItem("Choice three", OnChoice));
    // TODO Other types

    m_menu->AddItem(new GuiNestMenuItem("Nested menu >", childMenu));
  }

  return false;
}
} // namespace
