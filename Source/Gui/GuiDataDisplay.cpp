#include <limits>
#include "GuiDataDisplay.h"

namespace Amju
{
GuiDataDisplay::GuiDataDisplay()
{
  m_data = 0;
}

void GuiDataDisplay::Draw() 
{
  // Draw bg?
}

void GuiDataDisplay::SetData(ChartData* data)
{
  m_data = data;
}

ChartData* GuiDataDisplay::GetData()
{
  return m_data;
}
  
ChartData::ChartData()
{
  m_maxX = std::numeric_limits<XTYPE>::min();
  m_minX = std::numeric_limits<XTYPE>::max();
  m_numYVals = 0;
}

void ChartData::AddRowSimple(XTYPE x, YTYPE y)
{
  YVals yvals;
  yvals.push_back(y);
  AddRow(Row(x, yvals));  
}

void ChartData::AddRow(const ChartData::Row& r)
{
  m_numYVals = r.second.size();
  if (!m_rows.empty())
  {
    // Check num y vals same as previous row
    Assert(m_rows[0].second.size() == m_numYVals);
  }
  m_rows.push_back(r);

  // Check for new min/max values
  if (r.first > m_maxX)
  {
    m_maxX = r.first;
  }
  if (r.first < m_minX)
  {
    m_minX = r.first;
  }

  const YVals& y = r.second;
  if (m_maxY.empty())
  {
    m_maxY = y;
  }
  if (m_minY.empty())
  {
    m_minY = y;
  }
  
  for (unsigned int i = 0; i < y.size(); i++) 
  {
    if (y[i] > m_maxY[i])
    {
      m_maxY[i] = y[i];
    }
    if (y[i] < m_minY[i])
    {
      m_minY[i] = y[i];
    }
  }
}

}

