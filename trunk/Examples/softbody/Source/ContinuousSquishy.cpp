#include <DegRad.h>
#include "ContinuousSquishy.h"
#include "IntersectLine3Line3.h"

namespace Amju
{
// TODO TEMP TEST, value depends on scale of mesh.
// TODO Work it out based on bounding volume size maybe?
static const float MIN_SQ_DIST_TO_START_CUTTING = 0.2f; 

// When theta is larger than this, we treat the movement as a change in direction
static const float DIR_CHANGE_THRESH_THETA_RADIANS = DegToRad(30.0f);
static const float DIR_CHANGE_THRESH = cos(DIR_CHANGE_THRESH_THETA_RADIANS); 

static const float MIN_MOVE_SQ_DIST = 0.01f;

static const float HOLE_SIZE = MIN_SQ_DIST_TO_START_CUTTING * 0.75f; 

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

    Vec3f newEndPos = cutpoints[0].m_pos;
    Particle* p = GetParticle(m_spearhead[0]);
    Assert(p);
    Vec3f oldEndPos = p->GetPos();
    // Get direction from old endpoint to new endpoint. Is this the same direction as before, or are
    //  we changing direction?
    Vec3f cutDir = newEndPos - oldEndPos;
    if (cutDir.SqLen() > MIN_MOVE_SQ_DIST)
    {
      // Reposition the end point
      // NOT: 
      p->SetPos(newEndPos);
      // We have to use Reposition so the springs are updated - right??

      Vec3f cutDirNormalised = cutDir;
      cutDirNormalised.Normalise();

      // Compare with previous value, m_cutDir
      float dp = DotProduct(cutDirNormalised, m_cutDir);
      Assert(dp <=  1.00001f); // bah, fp precision
      Assert(dp >= -1.00001f);
      if (dp < DIR_CHANGE_THRESH)
      {
std::cout << "Direction change!\n";
        // Create new verts which are the new spearhead

        // Get the perpendicular to the cut, same as when we made the hole.
        //Vec3f perp = CrossProduct(cutpoints[0].m_tri->m_normal, cutDirNormalised);
        //perp.Normalise();
        //perp *= HOLE_SIZE;

        // TODO Rotate spearhead to face new direction

        // Don't change the spearhead particle IDs. Add new particles and connect
        // edges between the spearheads and what they used to connect to.
        int midId1 = AddNewParticle(GetParticle(m_spearhead[1])->GetPos()); 
        int midId2 = AddNewParticle(GetParticle(m_spearhead[2])->GetPos()); 
        AddEdge(midId1, m_spearhead[1]);
        AddEdge(m_spearhead[2], midId2);
      
        // Remove old edges m_following[0] -> m_spearhead[1] and m_following[1] -> m_spearhead[2]
        EraseSpring(m_following[0], m_spearhead[1]);
        EraseSpring(m_following[1], m_spearhead[2]);
        // Erasing these edges won't change topology, it just means these edges won't 
        //  change any more.

        // Add new edges from m_following[0] -> midId1 and m_following[1] -> midId2
        AddEdge(m_following[0], midId1);
        AddEdge(m_following[1], midId2);

        m_following[0] = midId1;
        m_following[1] = midId2;

        // Now the new cut dir is stored for next time
        m_cutDir = cutDirNormalised;
      }
      else
      {
        // No direction change. Reposition the other 2 spearhead verts
        //  (we already moved the end point).
        // NB Make sure we actually put these points the correct distance
        //  behind the end point, not just the difference between 2 cursor
        //  events. TODO
        Reposition(m_spearhead[1], cutDir);
        Reposition(m_spearhead[2], cutDir);

      }
    }
  }
  else
  {
    // Not cutting yet, the cursor has not moved far enough.
    // Get distance travelled from the start pos - large enough to start cutting?
    const CutPoint& cp = cutpoints[0];
    Vec3f dir = cp.m_pos - m_startCutPoint.m_pos;
    float sqdist = dir.SqLen();

//std::cout << "Sq dist: " << sqdist << "\n";

    if (sqdist > MIN_SQ_DIST_TO_START_CUTTING)
    {
      m_isCutting = true;
      m_cutDir = dir;
      m_cutDir.Normalise();

      // If we can start cutting, make the hexagonal hole. Remember the verts
      //  which will now move with the cursor
      MakeHole(m_startCutPoint, cp);
    }
  }
}

void ContinuousSquishy::EndCut(const LineSeg&, float cutDepth)
{
std::cout << "Finished!\n";

  if (!m_isCutting)
  {
    // Make minimum size hole
    // TODO
  }

  m_isCutting = false;
}

// Move the particle given by its ID, by the change in position vector.
// Make sure the new position is on the mesh surface.
// If we have crossed edges, tessellate as required (this will need extra info)
void ContinuousSquishy::Reposition(int id, const Vec3f& posChange)
{
  Particle* p = GetParticle(id);
  Assert(p);
  Vec3f pos = p->GetPos();
  pos += posChange;
  // TODO Make sure we are on surface.
  // TODO Have we crossed any edges? If so, tesselate.
  p->SetPos(pos);  

  // Adjust natural/min/max lengths of springs attached to this particle!
  // TODO
  SpringSet ss;
  GetSprings(id, &ss);
  Assert(!ss.empty()); // there must be springs attached, right?
  for (auto it = ss.begin(); it != ss.end(); ++it)
  {
    Spring* spr = *it;
    Particle* p1 = spr->GetParticle(0);
    Particle* p2 = spr->GetParticle(1);
    Assert(p1 == p || p2 == p);
    spr->ConnectToParticles(p1, p2); // recalcs natural len

    float natLen = spr->GetNaturalLength();
    // TODO TEMP TEST
    spr->SetMaxLength(natLen * 1.5f);
    spr->SetMinLength(natLen * 0.7f);
  }
}

void ContinuousSquishy::AddEdge(int startId, int endId)
{
  int sprId = CreateSpring(startId, endId);
  Spring* spr = GetSpring(sprId);

  float natLen = spr->GetNaturalLength();
  // TODO TEMP TEST
  spr->SetMaxLength(natLen * 1.5f);
  spr->SetMinLength(natLen * 0.7f);
}

static bool Intersects(const LineSeg& seg1, const LineSeg& seg2)
{
  LineSeg closest; 
  float mua = 0, mub = 0; // t values along segments 
  const float MIN_MU = 0.001f;
  const float MAX_MU = 0.999f;
  if (Intersects(seg1, seg2, &closest, &mua, &mub))
  {
    if (mua > MIN_MU && mua < MAX_MU && mub > MIN_MU && mub < MAX_MU)
    {
std::cout << "Found intersect, mua: " << mua << " mub: " << mub << "\n";
      return true;
    }
  }
  return false;
}

void ContinuousSquishy::MakeHole(
  const ContinuousSquishy::CutPoint& start, const ContinuousSquishy::CutPoint& end)
{
std::cout << "Making hole!\n";

  // Add new vert

  // Tessellate the tri - connect new vert to tri corners

  Vec3f cutDir = end.m_pos - start.m_pos;
  Vec3f midpos = (start.m_pos + end.m_pos) * 0.5f;
  Vec3f cutDirNorm = cutDir;
  cutDirNorm.Normalise();

  Tri* midTri = start.m_tri;
  Vec3f normal = midTri->m_normal; // normal to surface at mid point of hole
  if (start.m_tri != end.m_tri)
  {
    // Find the tri on which midpos lies - cast in dir of avg normal
    normal = (start.m_tri->m_normal + end.m_tri->m_normal) * 0.5f;
    normal.Normalise(); 

    static const float SEG_LENGTH = 2.0f; // How do we know how long to make the 'search' seg?
    // (We don't want to find a tri in front or behind!)
    LineSeg seg(midpos + normal * SEG_LENGTH, midpos - normal * SEG_LENGTH);
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
      // Make sure we are on tri surface, this would hopefully be a small adjustment, 
      //  but depends on scale of mesh I guess.
      midpos = cutpoints[0].m_pos;
    }
  }

  // Get the perpendicular to the cut - we have got the cut dir and normal, 
  //  so cross these two.
  // Use this to make some distance between the cut edges - although this 
  //  introduces problems, as now the hole could contain tris.
  // Let's assume the hole size is much smaller than the size of the tris.
  Vec3f perp = CrossProduct(normal, cutDir);
  perp.Normalise();
  perp *= HOLE_SIZE;

  Vec3f holeVert[6] = 
  {
    start.m_pos,
    midpos + perp - cutDirNorm * HOLE_SIZE, // TODO also offset along cut dir
    midpos - perp - cutDirNorm * HOLE_SIZE,
    midpos + perp + cutDirNorm * HOLE_SIZE, 
    midpos - perp + cutDirNorm * HOLE_SIZE,
    end.m_pos
  };

  int startId = AddNewParticle(holeVert[0]);
  int endId = AddNewParticle(holeVert[5]);
  int midId1 = AddNewParticle(holeVert[1]);
  int midId2 = AddNewParticle(holeVert[2]);

  m_spearhead[0] = endId;
  m_spearhead[1] = midId1;
  m_spearhead[2] = midId2;

  // Create the non-moving pair of the inner 4 verts.
  // Connect to start pos, tessellating as required.
  int midId3 = AddNewParticle(holeVert[3]);
  int midId4 = AddNewParticle(holeVert[4]);
  m_following[0] = midId3;
  m_following[1] = midId4;

  // Add edges - tesselating on one side, so we end up with a hole.
  AddEdge(startId, midId3);
  AddEdge(midId3, midId1);
  AddEdge(midId1, endId);
  AddEdge(endId, midId2);
  AddEdge(midId2, midId4);
  AddEdge(midId4, startId);

/*        3  1
          +--+
         /    \
  start +      + end
         \    /
          +--+
          4  2 
*/

  // Tessellate around the hole.
  // (We are assuming the hole is in ONE tri)
  // For each tri vert, 
  //   For each hole vert,
  //     Connect these 2 verts if there is LOS between them, i.e. we don't cross
  //     one of the hole edges.
  LineSeg holeEdges[6] = 
  {
    LineSeg(holeVert[0], holeVert[1]),
    LineSeg(holeVert[1], holeVert[2]),
    LineSeg(holeVert[2], holeVert[3]),
    LineSeg(holeVert[3], holeVert[4]),
    LineSeg(holeVert[4], holeVert[5]),
    LineSeg(holeVert[5], holeVert[0])
  };
  int holeVertId[6] = 
  {
    startId, midId3, midId1, endId, midId2, midId4
  };
  for (int i = 0; i < 3; i++)
  {
    int triVertId = midTri->m_particles[i];
    Vec3f triVertPos = GetParticle(triVertId)->GetPos();

    for (int j = 0; j < 6; j++)
    {
      bool yesEdge = true;
      // Add edge if it will not cross an edge of the hole
      LineSeg testEdge(triVertPos, holeVert[j]);
      for (int k = 0; k < 6; k++)
      {
        if (Intersects(holeEdges[k], testEdge)) 
        {
          yesEdge = false;
          break;
        }
      }
      if (yesEdge)
      {
        AddEdge(triVertId, holeVertId[j]);
      }
    }
  }

  // And now add the inside walls of the cut
  // TODO

  // TODO 
  // And put a spring between these points, to push the cut apart. Yuck.
  // (Maybe the start positions need some non-zero distance between them.)

}

}


