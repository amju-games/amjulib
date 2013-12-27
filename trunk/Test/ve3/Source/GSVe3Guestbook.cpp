#include <GuiFactory.h>
#include "GSVe3Guestbook.h"
#include "GuestbookWindow.h"

namespace Amju
{
static void OnBack()
{
  TheGSVe3Guestbook::Instance()->GoBack();
}

GSVe3Guestbook::GSVe3Guestbook()
{
}

void GSVe3Guestbook::Update()
{
  GSGui::Update();

}

void GSVe3Guestbook::Draw()
{
  GSGui::Draw();

}

void GSVe3Guestbook::Draw2d()
{
  GSGui::Draw2d();
}

void GSVe3Guestbook::SetPlayer(Player* player)
{
  m_player = player;
}

void GSVe3Guestbook::OnActive()
{
  GSGui::OnActive();

  // Add GuestbookWindow type to GUI factory 
  //  - then gui txt file can use GuestbookWindow directly
  static bool addOnce = TheGuiFactory::Instance()->Add("ve3-guestbook", CreateGuestbookWindow);
  m_gui = LoadGui("gui-ve3-guestbook.txt");
  Assert(m_gui);

  // Set title - "Your guestbook" or "<player>'s guestbook"
  // Colour scheme as in other gui states
  GuiText* name = (GuiText*)GetElementByName(m_gui, "title-text");
  Assert(name);
  std::string namestr;

  if (m_player->IsLocalPlayer())
  {
    namestr = "Your guestbook";
    name->SetFgCol(Colour(0, 0.5f, 0, 1));
  }
  else 
  {
    namestr = m_player->GetName() + "'s guestbook";
    if (m_player->IsLoggedIn())
    {
      name->SetFgCol(Colour(1, 0, 0, 1));
    }
    else
    {
      name->SetFgCol(Colour(0, 0, 0, 1));
    }
  }
  name->SetText(namestr);


  GuestbookWindow* gw = (GuestbookWindow*)m_gui->GetElementByName("my-guestbook");
  Assert(gw);
  gw->GetGBDisplay()->Init(m_player); 

  // TODO Set focus element, cancel element, command handlers
  GetElementByName(m_gui, "back-button")->SetCommand(OnBack);
}

} // namespace
