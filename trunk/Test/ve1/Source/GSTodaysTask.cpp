#include "GSTodaysTask.h"
#include <AmjuGL.h>

namespace Amju
{
GSTodaysTask::GSTodaysTask()
{
}

void GSTodaysTask::Update()
{
  GSGui::Update();

}

void GSTodaysTask::Draw()
{
  GSGui::Draw();

}

void GSTodaysTask::Draw2d()
{
  GSGui::Draw2d();
}

void GSTodaysTask::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("TODO");
  Assert(m_gui);

  // TODO Set focus element, cancel element, command handlers
}

} // namespace
