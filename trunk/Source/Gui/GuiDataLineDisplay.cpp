#include <AmjuFirst.h>
#include <AmjuGL.h>
#include <Colour.h>
#include <DrawRect.h>
#include "GuiDataLineDisplay.h"
#include "DrawBorder.h"
#include "GuiText.h"
#include <AmjuFinal.h>

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
  DrawFilled(this, Colour(0.8f, 0.8f, 0.8f, 1), 0);

  // TODO Draw grid lines?

  ChartData* data = GetData();
  if (!data)
  {
    return;
  }

  // Row of data, as if in a DB table
  int numRows = (int)data->GetNumRows();
  if (numRows == 0)
  {
std::cout << "No data for chart.\n";
    return;
  }

  const Vec2f& size = GetSize();
//  float barW = size.x / (float)numRows;

  // Get constants
  float xChartPos = GetCombinedPos().x + 0.2f; // TODO TEMP TEST offset
  float yChartPos = GetCombinedPos().y;
  float minX = (float)data->GetMinX();
  float xRange = (float)data->GetMaxX() - minX;
  if (xRange < 0.001f)
  {
    xRange = 1.0f;
    // TODO What abount negative range ??
  }
  Assert(xRange >= 0);
  // TODO Create a border area by changing these
  float xSize = size.x - 0.4; // remove 0.2 from sides for labels
  // Remove 0.1 off the bottom for x axis labels
  float ySize = size.y - 0.1f; // TODO 

  // Draw x axis labels
  for (int i = 0; i < numRows; i++)
  {
    const ChartData::Row& row = data->GetRow(i);
    // Get x-coord for this xval
    ChartData::XTYPE xVal = row.first;

    const std::string& str = data->GetXAxisLabel(i);
        
    static GuiText t;
    t.SetSize(Vec2f(1.0f, 0.1f));
    t.SetJust(GuiText::AMJU_JUST_CENTRE);
    t.SetDrawBg(false);
    float xPos = ((float)(xVal - minX)) / xRange * xSize + xChartPos;
    float yPos = yChartPos - ySize;
    t.SetLocalPos(Vec2f(xPos - 0.5f, yPos));
    t.SetText(str);
    t.SetTextSize(0.3f); // TODO 
    t.Draw();
  }

  // Draw lines

  // Loop for each column of yVals
  int numYvs = data->GetNumYVals();
  for (int yv = 0; yv < numYvs; yv++)
  {
    float minY = (float)data->GetMinY(yv);
    float yRange = (float)data->GetMaxY(yv) - minY;
    if (yRange < 0.001f)
    {
      yRange = 1.0f;
      // TODO What abount negative range ??
    }
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

