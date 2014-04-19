#include "GSViewObjGui.h"
#include <AmjuGL.h>

namespace Amju
{
GSViewObjGui::GSViewObjGui()
{
  //m_nextState=...
}

void GSViewObjGui::Update()
{
}

void GSViewObjGui::Draw()
{
}

void GSViewObjGui::Draw2d()
{
  m_gui->Draw();
//  TheCursorManager::Instance()->Draw(); // ?
}

void GSViewObjGui::OnActive()
{
  m_gui = LoadGui("gui-file-load-dialog.txt");
}

} // namespace
