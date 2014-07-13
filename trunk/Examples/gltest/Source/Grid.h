#ifndef GRID_H
#define GRID_H

#include <AmjuGL.h>
#include <TriList.h>

namespace Amju
{
class Grid
{
public:
  Grid();
  void Build(int numDivisions, float len);
  void Draw();

protected:
  RCPtr<TriListStatic> m_triList;
};
}

#endif
