#include <AmjuGL.h>
#include <Colour.h>
#include <DrawRect.h>
#include "GuiDataLineDisplay.h"
#include "DrawBorder.h"

namespace Amju
{
const char* GuiDataLineDisplay::NAME = "data-line-display";

GuiElement* CreateDataLineDisplay()
{
  return new GuiDataLineDisplay;
}

bool GuiDataLineDisplay::Load(File* f)
{
  // Get name, pos, size
  if (!GuiElement::Load(f))
  {
    return false;
  }

  return true;
}

void GuiDataLineDisplay::Draw()
{
  // TODO TEMP TEST Draw background
  DrawFilled(this, Colour(1, 1, 1, 1), 0);

  ChartData test;
  test.AddRowSimple(2, 5);
  test.AddRowSimple(3, 7);
  test.AddRowSimple(-1, 4);
  test.AddRowSimple(7, 2);

  ChartData* data = &test; // TODO GetData();
  if (!data)
  {
    return;
  }

  // Get max y-value
  // NB we can display multiple y values in chart - TODO
  // NB It might make sense to precalc meta data when the data is changed

  int numRows = (int)data->GetNumRows();
  if (numRows == 0)
  {
std::cout << "No data for chart.\n";
    return;
  }

  const Vec2f& size = GetSize();
//  float barW = size.x / (float)numRows;

  // Get constants
  float xChartPos = GetCombinedPos().x;
  float yChartPos = GetCombinedPos().y;
  float minX = (float)data->GetMinX();
  float xRange = (float)data->GetMaxX() - minX;
  Assert(xRange >= 0);
  // TODO Create a border area by changing these
  float xSize = size.x;
  float ySize = size.y;

  // Loop for each column of yVals
  int numYvs = data->GetNumYVals();
  for (int yv = 0; yv < numYvs; yv++)
  {
    float minY = (float)data->GetMinY(yv);
    float yRange = (float)data->GetMaxY(yv) - minY;
    Assert(yRange >= 0);

    // Draw each data row. TODO colours for each row..? Or auto generate
    for (int i = 0; i < numRows; i++)
    {
      const ChartData::Row& row = data->m_rows[i]; // TODO GetRow()
      // Get x-coord for this xval
      ChartData::XTYPE xVal = row.first;
      ChartData::YTYPE yVal = row.second[yv];

      float xPos = ((float)(xVal - minX)) / xRange * xSize + xChartPos;
      float yPos = ((float)(yVal - minY)) / yRange * ySize + yChartPos - ySize;
 
std::cout 
  << "Chart row " << i 
  << " xVal: " << xVal 
  << " xPos: " << xPos 
  << " yVal[" << yv << "]: " << yVal 
  << " yPos[" << yv << "]: " << yPos 
  << " xSize: " << xSize 
  << " ySize: " << ySize 
  << " xChartPos: " << xChartPos 
  << " yChartPos: " << yChartPos 
  << "\n";

       static const float S = 0.01f;
       Rect rect(xPos - S, xPos + S, yPos - S, yPos + S);
       PushColour();
       AmjuGL::SetColour(Colour(1, 0, 0, 1));
       AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
       DrawSolidRect(rect);
       AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
       PopColour();
    }
  }
}

}

