#include "GSVe3Guestbook.h"
#include <AmjuGL.h>

namespace Amju
{
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

void GSVe3Guestbook::OnActive()
{
  GSGui::OnActive();

  // Add GuestbookWindow type to GUI factory - but only if not already added, so use static
  //  - then gui txt file can use GuestbookWindow directly

  m_gui = LoadGui("TODO");
  Assert(m_gui);



  // TODO Set focus element, cancel element, command handlers
}

} // namespace
