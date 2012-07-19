#include "Screen.h"
#include <AmjuAssert.h>
#include <AmjuGL.h>

namespace Amju
{
#ifdef GEKKO
int Screen::s_x = 640;
int Screen::s_y = 480;
#else // GEKKO
int Screen::s_x = 0;
int Screen::s_y = 0;
#endif // GEKKO

int Screen::X() 
{ 
  Assert(s_x);
  return s_x;
}

int Screen::Y()
{
  Assert(s_y);
  return s_y;
}

void Screen::SetSize(int x, int y)
{
  s_x = x;
  s_y = y;
}

void SetViewportN(float x, float y, float w, float h)
{
  AmjuGL::Viewport(
    (x + 1.0f) * Screen::X() * 0.5f, 
    (y + 1.0f) * Screen::Y() * 0.5f, 
    w * (float)Screen::X() * 0.5f,
    h * (float)Screen::Y() * 0.5f);
}

void GetViewportN(float* x, float* y, float* w, float* h)
{
  int a, b, c, d;
  AmjuGL::GetViewport(&a, &b, &c, &d);
  *x = a / Screen::X() * 2.0f - 1.0f;
  *y = b / Screen::Y() * 2.0f - 1.0f;
  *w = c / Screen::X() * 2.0f;
  *h = d / Screen::Y() * 2.0f;
}
}
