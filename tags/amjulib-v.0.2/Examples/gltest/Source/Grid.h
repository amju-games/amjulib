#ifndef GRID_H
#define GRID_H

#include <AmjuGL.h>

namespace Amju
{
class Grid
{
public:
  Grid();
  void Build(int numDivisions, float len);
  void Draw();

protected:
  AmjuGL::Tris m_tris;
};
}

#endif
