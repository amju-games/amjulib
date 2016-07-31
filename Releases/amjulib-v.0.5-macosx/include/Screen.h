#ifndef AMJU_SCREEN_H
#define AMJU_SCREEN_H

namespace Amju
{
class Screen
{
public:
  static int X();
  static int Y();
  static void SetSize(int x, int y);

private:
  static int s_x, s_y;
};

// Get and set Viewport using normalised coords (-1..1). Full width or height of screen is 2.0
void SetViewportN(float x, float y, float w, float h);
void GetViewportN(float* x, float* y, float* w, float* h);
}

#endif
