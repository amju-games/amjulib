#ifndef TEAPOT_H
#define TEAPOT_H

#include <TriList.h>

namespace Amju
{
class Teapot
{
public:
  Teapot(float scale = 1.0f);
  void Draw();
private:
  RCPtr<TriListStatic> m_triList;
};
}

#endif  // TEAPOT_H

