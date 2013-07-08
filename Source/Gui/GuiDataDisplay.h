#ifndef GUI_DATA_DISPLAY_H_INCLUDED
#define GUI_DATA_DISPLAY_H_INCLUDED

#include <vector>
#include <Colour.h>
#include "GuiElement.h"

namespace Amju
{

// TODO Template, so we can have floats, ints, etc. Or use Variable ?
// Chart data is comprised of rows. Each Row has one 'x value' and a vector
//  of 'y values' (typically just one y value?)
class ChartData
{
public:
  typedef int XTYPE;
  typedef int YTYPE;

  typedef std::vector<YTYPE> YVals;
  typedef std::pair<XTYPE, YVals> Row;
  typedef std::vector<Row> Rows;
  typedef std::vector<std::string> Labels;

  ChartData();

  const Row& GetRow(int i) const { return m_rows[i]; }

  unsigned int GetNumRows() const { return m_rows.size(); }

  void AddRow(const Row& r);
  void AddXAxisLabel(const std::string& label);

  const std::string& GetXAxisLabel(int i) const { return m_xAxisLabels[i]; }

  // Use to simply add a row for simple (x, y) data (not multiple Y vals)
  void AddRowSimple(XTYPE x, YTYPE y);

  int GetNumYVals() const { return m_numYVals; }
  YTYPE GetMinY(int i) const { return m_minY[i]; }
  YTYPE GetMaxY(int i) const { return m_maxY[i]; }
  XTYPE GetMinX() const { return m_minX; }
  XTYPE GetMaxX() const { return m_maxX; }

private:
  Rows m_rows;

  // Meta data, used for drawing. Updated when we add a new row.
  YVals m_maxY;
  YVals m_minY;
  XTYPE m_minX, m_maxX;
  // Once the first row is added, all other rows must have this same
  //  number of y vals.
  int m_numYVals;

  Labels m_xAxisLabels;

  // If set, all y values are within the same (min, max) range
  // If not set, each line has its own min and max.
  bool m_useSameMinMax;
};

class GuiDataDisplay : public GuiElement
{
public:
  GuiDataDisplay();

  virtual void Draw() = 0;
  void SetData(ChartData*);
  ChartData* GetData();

  // Each 'column' of data (if you think of the data as rows in a DB table)
  //  is drawn in a different colour
  const Colour& GetColour(int dataColumn);

protected:
  ChartData* m_data;

  // Data points: we can mouse over each one
  struct DataPoint
  {
    Rect m_rect;
    std::string m_val;
    Colour m_colour; // ?
  };
  std::vector<DataPoint> m_dataPoints;
};

}

#endif

