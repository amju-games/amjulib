#include "GuiWindow.h"
#include <AmjuGL.h>
#include <Screen.h>
#include <DrawRect.h>

namespace Amju
{
const char* GuiWindow::NAME = "gui-window";

bool GuiWindow::Load(File* f)
{
  // Load name, pos, size
  if (!GuiElement::Load(f))
  {
    return false;
  }
  return LoadChildren(f);
}

void GuiWindow::Draw()
{
#ifdef WINDOW_DEBUG
  Rect r = GetRect(this);
  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
  DrawRect(r);
  AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
#endif

  int vp[4];
  AmjuGL::GetViewport(&vp[0], &vp[1], &vp[2], &vp[3]);

  SetViewportN(m_pos.x, m_pos.y -  m_size.y, m_size.x, m_size.y);

  // Now this element should fill whole viewport. Map the top left coord to (-1, 1) and
  //  bottom right to (1, -1).
  AmjuGL::PushMatrix();
  AmjuGL::Translate(m_pos.x * -2.0f / m_size.x - 1.0f, 1.0f - m_pos.y * 2.0f / m_size.y, 0);
  AmjuGL::Scale(2.0f / m_size.x, 2.0f / m_size.y, 1.0f);

  GuiComposite::Draw();
  AmjuGL::PopMatrix();
  AmjuGL::Viewport(vp[0], vp[1], vp[2], vp[3]);
}
}

