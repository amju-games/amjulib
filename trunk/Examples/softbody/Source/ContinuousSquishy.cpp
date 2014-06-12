#include "ContinuousSquishy.h"

namespace Amju
{
static const float MIN_SQ_DIST_TO_START_CUTTING = 1.0f; // TODO TEMP TEST

ContinuousSquishy::ContinuousSquishy() : m_isCutting(false)
{
}

void ContinuousSquishy::StartCut(const LineSeg& seg, float cutDepth)
{
  m_isCutting = false; // set to true once we move far enough

  // Get the position and Tri for the cursor pos
  // Remember this pos!
  CutPoints cutpoints;
  GetCutPoints(seg, &cutpoints);
  if (cutpoints.empty())
  {
    return;
  }

  m_startCutPoint = cutpoints[0];

  // Maybe that's all we need to do? We create the initial hexagonal cut later when the
  //  mouse has moved far enough...?
}

void ContinuousSquishy::ContinueCut(const LineSeg& seg, float cutDepth)
{
  // Get cut point on mesh
  CutPoints cutpoints;
  GetCutPoints(seg, &cutpoints);
  if (cutpoints.empty())
  {
    return;
  }

  // Are we cutting yet?
  if (m_isCutting)
  {
    // Yes, we are cutting

    // Get direction from old endpoint to new endpoint. Is this the same direction as before, or are
    //  we changing direction?


    // Reposition the moving verts
    Vec3f newEndPos = cutpoints[0].m_pos;
    Particle* p = GetParticle(m_spearhead[0]);
    Assert(p);
    p->SetPos(newEndPos);
//std::cout << "Cutting.... repositioning spearhead of cut..\n";

    // Has this vert crossed an edge?

    // Should we check if we should change direction?
  }
  else
  {
    // Not cutting yet, the cursor has not moved far enough.
    // Get distance travelled from the start pos - large enough to start cutting?
    const CutPoint& cp = cutpoints[0];
    float sqdist = (cp.m_pos - m_startCutPoint.m_pos).SqLen();

//std::cout << "Sq dist: " << sqdist << "\n";

    if (sqdist > MIN_SQ_DIST_TO_START_CUTTING)
    {
      m_isCutting = true;

      // If we can start cutting, make the hexagonal hole. Remember the verts
      //  which will now move with the cursor
      MakeHole(m_startCutPoint, cp);
    }
  }
}

void ContinuousSquishy::EndCut(const LineSeg&, float cutDepth)
{
std::cout << "Finished!\n";

  m_isCutting = false;

  // Do we actually need to do anything here? :-)
}

void ContinuousSquishy::MakeHole(
  const ContinuousSquishy::CutPoint& start, const ContinuousSquishy::CutPoint& end)
{
std::cout << "Making hole!\n";

  // Add new vert

  // Tessellate the tri - connect new vert to tri corners


  Vec3f midpos = (start.m_pos + end.m_pos) * 0.5f;
  Tri* midTri = start.m_tri;
  if (start.m_tri != end.m_tri)
  {
    // Find the tri on which midpos lies - cast in dir of avg normal
    Vec3f avgNorm = (start.m_tri->m_normal + end.m_tri->m_normal) * 0.5f;
    avgNorm.Normalise(); // really necessary?
    static const float SEG_LENGTH = 2.0f; // How do we know how long to make the 'search' seg?
    // (We don't want to find a tri in front or behind!)
    LineSeg seg(midpos + avgNorm * SEG_LENGTH, midpos - avgNorm * SEG_LENGTH);
    CutPoints cutpoints;
    GetCutPoints(seg, &cutpoints);
    if (cutpoints.empty())
    {
      std::cout << "Crap, can't find tri for mid point. Is there a hole??\n";
      // Stick with start tri I guess.
    }
    else
    {
      midTri = cutpoints[0].m_tri;
      std::cout << "Found mid pos, adjusting from " << midpos << " to " << cutpoints[0].m_pos << "\n";
      // Make sure we are tri surface, this would hopefully be a small adjustment, 
      //  but depends on scale of mesh I guess.
      midpos = cutpoints[0].m_pos;
    }
  }

  int startId = AddNewParticle(start.m_pos);
  int endId = AddNewParticle(end.m_pos);
  int midId1 = AddNewParticle(midpos);
  int midId2 = AddNewParticle(midpos);

  m_spearhead[0] = endId;
  m_spearhead[1] = midId1;
  m_spearhead[2] = midId2;
}

}


