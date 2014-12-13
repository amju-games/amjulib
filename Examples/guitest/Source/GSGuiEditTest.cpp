#include "GSGuiEditTest.h"
#include <AmjuGL.h>

namespace Amju
{
GSGuiEditTest::GSGuiEditTest()
{
}

void GSGuiEditTest::Update()
{
  GSGui::Update();

}

void GSGuiEditTest::Draw()
{
  GSGui::Draw();

}

void GSGuiEditTest::Draw2d()
{
  GSGui::Draw2d();
}

void GSGuiEditTest::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-edit-test.txt");
  Assert(m_gui);
}

bool GSGuiEditTest::OnCursorEvent(const CursorEvent& ce)
{
  return false;
}

bool GSGuiEditTest::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  return false;
}
} // namespace
