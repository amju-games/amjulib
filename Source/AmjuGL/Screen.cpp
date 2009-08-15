#include "Screen.h"
#include "AmjuAssert.h"

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
}
