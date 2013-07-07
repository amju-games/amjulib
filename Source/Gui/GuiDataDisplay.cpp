#include <AmjuFirst.h>
#include <limits>
#include "GuiDataDisplay.h"
#include <AmjuFinal.h>

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
  m_useSameMinMax = true; // TODO TEMP TEST
}

void ChartData::AddRowSimple(XTYPE x, YTYPE y)
{
  YVals yvals;
  yvals.push_back(y);
  AddRow(Row(x, yvals));  
}

void ChartData::AddXAxisLabel(const std::string& label)
{
  m_xAxisLabels.push_back(label);
}

void ChartData::AddRow(const ChartData::Row& r)
{
  m_numYVals = r.second.size();
  if (!m_rows.empty())
  {
    // Check num y vals same as previous row
    Assert((int)m_rows[0].second.size() == m_numYVals);
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
    for (unsigned int j = 0; j < y.size(); j++)
    {
      m_maxY.push_back(std::numeric_limits<XTYPE>::min());
    }
  }
  if (m_minY.empty())
  {
    for (unsigned int j = 0; j < y.size(); j++)
    {
      m_minY.push_back(std::numeric_limits<XTYPE>::max());
    }
  }
  
  if (m_useSameMinMax)
  {
    // Set all min values to the same min y value
    // Set all max values to the same max y value
    for (unsigned int i = 0; i < y.size(); i++) 
    {
      if (y[i] > m_maxY[0])
      {
        for (unsigned int j = 0; j < m_maxY.size(); j++)
        {
          m_maxY[j] = y[i];
        }
      }
      if (y[i] < m_minY[0])
      {
        for (unsigned int j = 0; j < m_maxY.size(); j++)
        {
          m_minY[j] = y[i];
        }
      }
    }
  }
  else
  {
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

}

