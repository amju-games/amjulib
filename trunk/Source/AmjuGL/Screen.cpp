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
    int((x + 1.0f) * (float)Screen::X() * 0.5f), 
    int((y + 1.0f) * (float)Screen::Y() * 0.5f), 
    int(w * (float)Screen::X() * 0.5f),
    int(h * (float)Screen::Y() * 0.5f));
}

void GetViewportN(float* x, float* y, float* w, float* h)
{
  int a, b, c, d;
  AmjuGL::GetViewport(&a, &b, &c, &d);
  *x = a / (float)Screen::X() * 2.0f - 1.0f;
  *y = b / (float)Screen::Y() * 2.0f - 1.0f;
  *w = c / (float)Screen::X() * 2.0f;
  *h = d / (float)Screen::Y() * 2.0f;
}
}
