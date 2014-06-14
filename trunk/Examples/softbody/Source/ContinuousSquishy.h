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

  // Move the particle given by its ID, by the change in position vector.
  // Make sure the new position is on the mesh surface.
  // If we have crossed edges, tessellate as required (this will need extra info)
  void Reposition(int id, const Vec3f& posChange);

  // Add edge connecting the two particles given by their IDs.
  void AddEdge(int startId, int endId);

protected:
  CutPoint m_startCutPoint;
  bool m_isCutting; // once cut started and we have moved min distance

  // IDs of the 3 particles forming the leading (bleeding!) end of the cut.
  // Element 0 is the end point, elements 1 and 2 are connected to the end point.
  int m_spearhead[3];
  // The two particles 'following' these 3.
  // m_following[0] is connected to m_spearhead[1]
  // m_following[1] is connected to m_spearhead[2]
  int m_following[2];

  // Direction of cut - initially dir from start to end, then the dir
  //  from the previous to the latest end point.
  Vec3f m_cutDir;
};
}

