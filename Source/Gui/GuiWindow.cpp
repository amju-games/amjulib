#include "GuiWindow.h"
#include <AmjuGL.h>
#include <Screen.h>
#include <DrawRect.h>

#define NESTED_VIEWPORTS

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

#ifdef NESTED_VIEWPORTS

std::cout << "Window: " << m_name << ": [Combined] pos: x: " << pos.x << " y: " << pos.y << "\n";

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
  if (x < nvp[0])
  {
    x = nvp[0]; // choose rightmost
    w -= (nvp[0] - pos.x); // reduce width by left hand excess
  }
  if (x + w > nvp[0] + nvp[2])  
  {
    w = nvp[2] - x;
  }
  if (w < 0)
  {
    w = 0;
  }

  float y = pos.y - size.y; 
  float h = size.y;
  if (y < nvp[1]) // TODO Check this
  {
    y = nvp[1]; 
    h -= (nvp[1] - y); 
  }
  if (y + h > nvp[1] + nvp[3])
  {
    h = nvp[3] - (y + h);
  }
  if (h < 0)
  {
    h = 0;
  }

std::cout << "Window: " << m_name << ": setting new viewpt: x: " << x << " y: " << y << " w: " << w << " h: " << h << "\n";

  //SetViewportN(pos.x, pos.y -  size.y, size.x, size.y);
  SetViewportN(x, y, w, h);

  // Now this element should fill whole viewport. Map the top left coord (pos.x, pos.y + size.y) to (-1, 1) and
  //  bottom right (pos.x + size.x, pos.y) to (1, -1).
  AmjuGL::PushMatrix();

  float trx1 = pos.x * -2.0f / size.x - 1.0f; // ok
  float try1 = 1.0f - pos.y * 2.0f / size.y;

  float trx2 = x * -2.0f / w - 1.0f;
  float try2 = 1.0f - (y + h) * 2.0f / h;


//  AmjuGL::Translate(pos.x * -2.0f / size.x - 1.0f, 1.0f - pos.y * 2.0f / size.y, 0);
//  AmjuGL::Scale(2.0f / size.x, 2.0f / size.y, 1.0f);

//  AmjuGL::Translate(x * -2.0f / w - 1.0f, 1.0f - y * 2.0f / h, 0);

  AmjuGL::Translate(trx2, try2, 0);
  
  AmjuGL::Scale(2.0f / w, 2.0f / h, 1.0f);

  GuiComposite::Draw();
  AmjuGL::PopMatrix();

#else // NESTED_VIEWPORTS

  SetViewportN(pos.x, pos.y -  size.y, size.x, size.y);
  AmjuGL::PushMatrix();
  AmjuGL::Translate(pos.x * -2.0f / size.x - 1.0f, 1.0f - pos.y * 2.0f / size.y, 0);
  AmjuGL::Scale(2.0f / size.x, 2.0f / size.y, 1.0f);

  GuiComposite::Draw();
  AmjuGL::PopMatrix();

#endif // NESTED_VIEWPORTS

  AmjuGL::Viewport(vp[0], vp[1], vp[2], vp[3]);
}
}

