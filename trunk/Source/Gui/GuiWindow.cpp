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

  // Set viewport so anything outside of the boundary will be clipped. But make sure
  //  the new viewport is no larger than any existing viewport set!
  // So, get the current veiwport in normalised form, then do min/max to make sure
  //  we don't draw outside the boundary for any ancestor nodes.
  float nvp[4]; // normalised viewport
  GetViewportN(&nvp[0], &nvp[1], &nvp[2], &nvp[3]); // x, y, w, h

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

  SetViewportN(x, y, w, h);
  m_clippedRect.Set(x, x + w, y, y + h);

  // Now this element should fill whole viewport. 
  // Map the top left coord (pos.x, pos.y + size.y) to (-1, 1) and
  //  bottom right (pos.x + size.x, pos.y) to (1, -1).
  AmjuGL::PushMatrix();

  // Scale factor. The whole range should be -1..+1, so we want to map the range of coords
  //  to a total range of 2.0.
  float sx = 2.0f / w;
  float sy = 2.0f / h;
  // Translate so that the min x coord maps to -1: 
  //  trx + minx * sx = -1   => trx = minx * -sx - 1
  // OR Equally, trx + maxx* sx = 1   => trx = maxx * -sx + 1
  float trX = x * -sx - 1.0f;
  float trY = 1.0f - (y + h) * 2.0f / h; // same idea but y is upside down

  // Sadly if this window is nested, there is ALREADY a translate and scale factor in effect!
  // Hmm, this "works" but will trash any transformation NOT due to nested windowing.
  AmjuGL::SetIdentity(); // whoops, there goes the old scale and translate

  AmjuGL::Translate(trX, trY, 0);  
  AmjuGL::Scale(sx, sy, 1.0f);

  GuiComposite::Draw();
  AmjuGL::PopMatrix();

#else // NESTED_VIEWPORTS

  SetViewportN(pos.x, pos.y -  size.y, size.x, size.y);
  AmjuGL::PushMatrix();
  // Translate so that the min x coord maps to -1: 
  //  trx + minx * sx = -1   => trx = minx * -sx - 1
  AmjuGL::Translate(pos.x * -2.0f / size.x - 1.0f, 1.0f - pos.y * 2.0f / size.y, 0);
  AmjuGL::Scale(2.0f / size.x, 2.0f / size.y, 1.0f);

  GuiComposite::Draw();
  AmjuGL::PopMatrix();

#endif // NESTED_VIEWPORTS

  AmjuGL::Viewport(vp[0], vp[1], vp[2], vp[3]);
}

bool GuiWindow::OnDoubleClickEvent(const DoubleClickEvent& e)
{
  /*
  // Discard if outside Window
  if (!m_clippedRect.IsPointIn(Vec2f(e.x, e.y)))
  {
    return false;
  }
  */

  bool ret = false;
  int s = m_children.size();
  for (int i = 0; i < s; i++)
  {
    if (m_children[i]->OnDoubleClickEvent(e))
    {
      ret = true;
    }
  }
  return ret;
}

bool GuiWindow::OnMouseButtonEvent(const MouseButtonEvent& e)
{
  /*
  // Discard mouse down event if outside Window
  if (e.isDown && !m_clippedRect.IsPointIn(Vec2f(e.x, e.y)))
  {
    return false;
  }
  */

  bool ret = false;
  int s = m_children.size();
  for (int i = 0; i < s; i++)
  {
    if (m_children[i]->OnMouseButtonEvent(e))
    {
      ret = true;
    }
  }
  return ret;
}

bool GuiWindow::OnCursorEvent(const CursorEvent& e)
{
  /*
  // Discard if outside Window ?
  if (!m_clippedRect.IsPointIn(Vec2f(e.x, e.y)))
  {
//    return false;
  }
  */

  bool ret = false;
  int s = m_children.size();
  for (int i = 0; i < s; i++)
  {
    if (m_children[i]->OnCursorEvent(e))
    {
      ret = true;
    }
  }
  return ret;
}

}

