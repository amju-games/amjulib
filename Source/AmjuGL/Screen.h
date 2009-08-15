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
}

#endif
