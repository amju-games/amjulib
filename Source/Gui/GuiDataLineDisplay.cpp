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
  static const Colour BG_COLOUR = Colour(0.8f, 0.8f, 0.8f, 1);
  static const Colour GRID_COLOUR = Colour(0.95f, 0.95f, 0.95f, 1);

  DrawFilled(this, BG_COLOUR, 0);

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
//std::cout << "No data for chart.\n";
    return;
  }

  const Vec2f& size = GetSize();
//  float barW = size.x / (float)numRows;

  static const float X_BORDER = 0.2f;
  static const float Y_BORDER = 0.1f;

  // Get constants
  float xChartPos = GetCombinedPos().x + X_BORDER; 
  float yChartPos = GetCombinedPos().y - Y_BORDER;
  float minX = (float)data->GetMinX();
  float xRange = (float)data->GetMaxX() - minX;
  if (xRange < 0.001f)
  {
    xRange = 1.0f;
    // TODO What abount negative range ??
  }
  Assert(xRange >= 0);
  // TODO Create a border area by changing these
  float xSize = size.x - X_BORDER * 2.0f; // remove 0.2 from sides for labels
  // Remove 0.1 off the bottom for x axis labels
  float ySize = size.y - Y_BORDER * 2.0f;  

  // Draw grid
  PushColour();
  AmjuGL::SetColour(GRID_COLOUR);
  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
  DrawLine(Vec2f(xChartPos, yChartPos), Vec2f(xChartPos + xSize, yChartPos));
  DrawLine(Vec2f(xChartPos, yChartPos - ySize), Vec2f(xChartPos + xSize, yChartPos - ySize));
  AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
  PopColour();

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
    t.SetFontSize(0.3f); // TODO 
    t.Draw();
  }

  // Draw data points and lines
  m_dataPoints.clear();

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
       AmjuGL::SetColour(GetColour(yv));
       AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
       DrawSolidRect(rect);

       Vec2f lineSegEnd(xPos, yPos);
       if (i > 0)
       {
         DrawLine(lineSegEnd, prevLineSegEnd);
       }
       AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
       PopColour();

       static GuiText t;
       t.SetSize(Vec2f(1.0f, 0.1f));
       t.SetJust(GuiText::AMJU_JUST_CENTRE);
       t.SetDrawBg(false);
       t.SetLocalPos(Vec2f(xPos - 0.5f, yPos + 0.1f));
       t.SetText(ToString(yVal));
       t.SetFontSize(0.3f); // TODO 
       t.Draw();

       DataPoint dp;
       dp.m_rect = rect;
       dp.m_colour = GetColour(yv);
       dp.m_val = ToString(yVal);
       m_dataPoints.push_back(dp);

       prevLineSegEnd = lineSegEnd;
    }
  }
}

bool GuiDataLineDisplay::OnCursorEvent(const CursorEvent& ce)
{
  int s = m_dataPoints.size();
  for (int i = 0; i < s; i++)
  {
    DataPoint& dp = m_dataPoints[i];
    if (dp.m_rect.IsPointIn(Vec2f(ce.x, ce.y)))
    {
      // Show full info for this data point
      // TODO
    }
  }
  return false;
}

}

