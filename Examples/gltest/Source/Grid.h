#ifndef GRID_H
#define GRID_H

#include <AmjuGL.h>
#include <TriList.h>

namespace Amju
{
// Function which returns a y value for an (x, z) coordinate
typedef float (*HeightFunc)(float, float);

class Grid
{
public:
  Grid();
  void Build(int numDivisions, float len, HeightFunc hf = 0);
  void Draw();

protected:
  RCPtr<TriListStatic> m_triList;
};
}

#endif
