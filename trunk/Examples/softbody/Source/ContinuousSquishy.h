#pragma once

#include "Squishy.h"

namespace Amju
{
class ContinuousSquishy : public Squishy
{
public:
  ContinuousSquishy();

  virtual void StartCut(const LineSeg&, float cutDepth);
  virtual void ContinueCut(const LineSeg&, float cutDepth);
  virtual void EndCut(const LineSeg&, float cutDepth);

protected:
  void MakeHole(const CutPoint& start, const CutPoint& end);

protected:
  CutPoint m_startCutPoint;
  bool m_isCutting; // once cut started and we have moved min distance

  // IDs of the 3 particles forming the leading (bleeding!) end of the cut.
  // Element 0 is the end point, elements 1 and 2 are connected to the end point.
  int m_spearhead[3];
};
}

