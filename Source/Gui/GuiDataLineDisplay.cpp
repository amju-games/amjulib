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

// TODO Thickness; draw area under line
void DrawLine(const Vec2f& v1, const Vec2f& v2)
{
  AmjuGL::DrawLine(AmjuGL::Vec3(v1.x, v1.y, 0), AmjuGL::Vec3(v2.x, v2.y, 0));
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
  // Draw background - TODO Colour
  DrawFilled(this, Colour(1, 1, 1, 1), 0);

  // TODO Draw grid lines?

  ChartData* data = GetData();
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
    Vec2f prevLineSegEnd; // point in previous iteration, to draw line 
    for (int i = 0; i < numRows; i++)
    {
      const ChartData::Row& row = data->GetRow(i);
      // Get x-coord for this xval
      ChartData::XTYPE xVal = row.first;
      ChartData::YTYPE yVal = row.second[yv];

      float xPos = ((float)(xVal - minX)) / xRange * xSize + xChartPos;
      float yPos = ((float)(yVal - minY)) / yRange * ySize + yChartPos - ySize;

#ifdef CHART_DEBUG 
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
#endif

       static const float S = 0.01f;
       Rect rect(xPos - S, xPos + S, yPos - S, yPos + S);
       PushColour();
       AmjuGL::SetColour(Colour(1, 0, 0, 1));
       AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
       DrawSolidRect(rect);

       Vec2f lineSegEnd(xPos, yPos);
       if (i > 0)
       {
         DrawLine(lineSegEnd, prevLineSegEnd);
       }
       AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
       PopColour();

       prevLineSegEnd = lineSegEnd;
    }
  }
}

}

