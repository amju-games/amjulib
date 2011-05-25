#ifndef TEAPOT_H
#define TEAPOT_H

#include <AmjuGL.h>

namespace Amju
{
class Teapot
{
public:
  Teapot(float scale = 1.0f);
  void Draw();
private:
  AmjuGL::Tris m_tris;
};
}

#endif  // TEAPOT_H

