#include <AmjuFirst.h>
#include "Screen.h"
#include <AmjuAssert.h>
#include <AmjuGL.h>
#include <AmjuFinal.h>

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
  float screenX = (float)Screen::X();
  float screenY = (float)Screen::Y();

  AmjuGL::Viewport(
    int((x + 1.0f) * screenX * 0.5f), 
    int((y + 1.0f) * screenY * 0.5f), 
    int(w * screenX * 0.5f),
    int(h * screenY * 0.5f));
}

void GetViewportN(float* x, float* y, float* w, float* h)
{
  int a, b, c, d;
  AmjuGL::GetViewport(&a, &b, &c, &d);

  float screenX = (float)Screen::X();
  float screenY = (float)Screen::Y();

  *x = a / screenX * 2.0f - 1.0f;
  *y = b / screenY * 2.0f - 1.0f;
  *w = c / screenX * 2.0f;
  *h = d / screenY * 2.0f;
}
}
