#pragma once

#include "Squishy.h"

namespace Amju
{
class ContinuousSquishy : public Squishy
{
  struct HoleVert
  {
    HoleVert(int id = -1, PTri tri = 0) : m_id(id), m_tri(tri) {}
    int GetId() const { return m_id; }
    Tri* GetTri() { return m_tri; }

  private:
    int m_id; // particle ID
    PTri m_tri;
  };

public:
  ContinuousSquishy();

  virtual void StartCut(const LineSeg&, float cutDepth) override;
  virtual void ContinueCut(const LineSeg&, float cutDepth) override;
  virtual void EndCut(const LineSeg&, float cutDepth) override;

  virtual void Update() override;

protected:
  void MakeHole(const CutPoint& start, const CutPoint& end);

  // Move the particle given by its ID, by the change in position vector.
  // Make sure the new position is on the mesh surface.
  // If we have crossed edges, tessellate as required (this will need extra info)
  void Reposition(HoleVert*, const Vec3f& posChange);

  // Add edge connecting the two particles given by their IDs.
  void AddEdge(int startId, int endId);

protected:
  CutPoint m_startCutPoint;
  bool m_isCutting; // once cut started and we have moved min distance

  // IDs of the 3 particles forming the leading (bleeding!) end of the cut.
  // Element 0 is the end point, elements 1 and 2 are connected to the end point.
  HoleVert m_leading[3];

  // The two particles 'following' these 3.
  // m_following[0] is connected to m_leading[1]
  // m_following[1] is connected to m_leading[2]
  HoleVert m_following[2];

  // Direction of cut - initially dir from start to end, then the dir
  //  from the previous to the latest end point.
  Vec3f m_cutDir;
};
}

