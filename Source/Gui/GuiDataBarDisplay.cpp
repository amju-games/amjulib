#include <AmjuFirst.h>
#include "GuiDataBarDisplay.h"
#include <AmjuFinal.h>

namespace Amju
{
void GuiDataBarDisplay::Draw()
{
  // NB These bars are vertical
  // Work out width of each bar: width/num of x values
  // Get max y-value
  // NB we can display multiple y values in a bar chart - TODO
  // NB It might make sense to precalc meta data when the data is changed

  int numRows = GetData()->GetNumRows();
//  const Vec2f& size = GetSize();
//  float barW = size.x / (float)numRows;

  // Draw each bar. TODO colours for each row..? Or auto generate
  for (int i = 0; i < numRows; i++)
  {
  }
}

}

