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
  if (!IsVisible())
  {
    return;
  }

//#ifdef WINDOW_DEBUG
  // TODO make this a flag
  // Border
  Rect r = GetRect(this);
  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
  PushColour();
  AmjuGL::SetColour(1, 1, 1, 1); // TODO make configurable
  DrawRect(r);
  PopColour();
  AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
//#endif

  int vp[4];
  AmjuGL::GetViewport(&vp[0], &vp[1], &vp[2], &vp[3]);

  Vec2f size = GetSize();
  Vec2f pos = GetCombinedPos();

  // Set viewport so anything outside of the boundary will be clipped. But make sure
  //  the new viewport is no larger than any existing viewport set!
  // So, get the current veiwport in normalised form, then do min/max to make sure
  //  we don't draw outside the boundary for any ancestor nodes.
  float nvp[4]; // normalised viewport
  GetViewportN(&nvp[0], &nvp[1], &nvp[2], &nvp[3]); // x, y, w, h

std::cout << "Window: " << m_name << ": inherited viewport: x: " << nvp[0] << " y: " << nvp[1] << " w: " << nvp[2] << " h: " << nvp[3] << "\n";

  float x = pos.x; 
  // Chop w to fit inherited window width
  float w = size.x;
  if (pos.x < nvp[0])
  {
    x = nvp[0]; // choose rightmost
    w -= (nvp[0] - pos.x); // reduce width by left hand excess
  }
  if (w > nvp[2]) // TODO is this right ? 
  {
    w = nvp[2];
  }
  if (w < 0)
  {
    w = 0;
  }

  float y = pos.y - size.y; ///std::max(pos.y - size.y, nvp[1]); // TODO 
  float h = size.y;
  if (y < nvp[1]) // TODO Sort this
  {
    y = nvp[1]; // wrong
    h -= (nvp[1] - y); // wrong
  }
  if (h > nvp[3])
  {
    h = nvp[3];
  }
  if (h < 0)
  {
    h = 0;
  }

std::cout << "Window: " << m_name << ": setting new viewpt: x: " << x << " y: " << y << " w: " << w << " h: " << h << "\n";

  //SetViewportN(pos.x, pos.y -  size.y, size.x, size.y);
  SetViewportN(x, y, w, h);

  // Now this element should fill whole viewport. Map the top left coord to (-1, 1) and
  //  bottom right to (1, -1).
  AmjuGL::PushMatrix();
  AmjuGL::Translate(pos.x * -2.0f / size.x - 1.0f, 1.0f - pos.y * 2.0f / size.y, 0);
  AmjuGL::Scale(2.0f / size.x, 2.0f / size.y, 1.0f);

  GuiComposite::Draw();
  AmjuGL::PopMatrix();
  AmjuGL::Viewport(vp[0], vp[1], vp[2], vp[3]);
}
}

