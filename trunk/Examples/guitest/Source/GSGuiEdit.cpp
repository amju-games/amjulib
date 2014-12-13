#include <AmjuGL.h>
#include "GSGuiEdit.h"
#include "DragBox.h"

namespace Amju
{
GSGuiEdit::GSGuiEdit()
{
}

void GSGuiEdit::Update()
{
  GSGui::Update();

}

void GSGuiEdit::Draw()
{
  GSGui::Draw();

}

void GSGuiEdit::Draw2d()
{
  GSGui::Draw2d();
}

void GSGuiEdit::OnActive()
{
  GSGui::OnActive();

  DragBox* db = new DragBox; 
  db->SetColour(Colour(1, 0, 0, 1));
  db->SetSize(Vec2f(0.3f, 0.2f));
  m_gui = db;
  Assert(m_gui);
}

bool GSGuiEdit::OnCursorEvent(const CursorEvent& ce)
{
  return false;
}

bool GSGuiEdit::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  return false;
}
} // namespace
