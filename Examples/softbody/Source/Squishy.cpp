#include <CollisionMesh.h>
#include "Squishy.h"
#include "SquishLoader.h"
#include "IntersectLine3Line3.h"

#ifdef AMJU_USE_GLUT
#ifdef WIN32
#include <gl/glut.h>
#endif
#ifdef MACOSX
#include <GLUT/glut.h>
#endif
#endif

namespace Amju
{
/*
Ray-Tri Intersection
Adapted from http://www.lighthouse3d.com/tutorials/maths/ray-triangle-intersection/
Moller-Haines algorithm
*/
int rayIntersectsTriangle(
  const Vec3f& p, const Vec3f& d, 
  const Vec3f& v0, const Vec3f& v1, const Vec3f& v2,
  Vec3f* intersectionPoint)
{
  Vec3f e1 = v1 - v0;
  Vec3f e2 = v2 - v0;

  Vec3f h = CrossProduct(d, e2);

  float a = DotProduct(e1, h);

  if (a > -0.00001 && a < 0.00001)
    return(false);

  float f = 1.0f / a;
  Vec3f s = p - v0;

  float u = f * DotProduct(s, h);

  if (u < 0.0 || u > 1.0)
    return(false);

  Vec3f q = CrossProduct(s, e1);

  float v = f * DotProduct(d, q);

  if (v < 0.0 || u + v > 1.0)
    return(false);

  // at this stage we can compute t to find out where
  // the intersection point is on the line
  float t = f * DotProduct(e2, q);

  if (t > 0.00001) // ray intersection
  {
    *intersectionPoint = p + t * d; 
    return(true);
  }
  // this means that there is a line intersection but not a ray intersection
  return (false);
}


Squishy::Squishy()
{
  m_volume = 0;

 // m_mesh = nullptr; // TODO TEMP TEST
  m_drawSpringSystem = false;

  m_numVerts = 0;
}

struct CutPointSorter
{
  CutPointSorter(const Vec3f& p) : m_pos(p) {}
  bool operator()(const Squishy::CutPoint& cp1, const Squishy::CutPoint& cp2)
  {
    float sqdist1 = (cp1.m_pos - m_pos).SqLen();
    float sqdist2 = (cp2.m_pos - m_pos).SqLen();
    return sqdist1 < sqdist2;
  }
  Vec3f m_pos;
};

void Squishy::GetCutPoints(const LineSeg& seg, CutPoints* cutpoints)
{
  // Check each tri, does it intersect.
  // NB Could be more than one if we cut through the squishy
  // Get distance along line seg and intersection point for each tri

  // points where line seg intersects squishy - once we have tested all tris,
  //  find closest

  for (auto it = m_trilist.begin(); it != m_trilist.end(); ++it)
  {
    Tri* tri = *it;

    Particle* particle[3] = 
    {
      GetParticle(tri->m_particles[0]),
      GetParticle(tri->m_particles[1]),
      GetParticle(tri->m_particles[2])
    };
    Vec3f pos[3] = 
    {
      particle[0]->GetPos(),
      particle[1]->GetPos(),
      particle[2]->GetPos()
    };

    Vec3f intersect;
    int r = rayIntersectsTriangle(seg.p0, seg.p1 - seg.p0, pos[0], pos[1], pos[2], &intersect);
    tri->m_selected = (r != 0);
    if (r)
    {
      //std::cout << "Got intersection!: " << intersect.x << " " << intersect.y << " " << intersect.z << "\n";
      CutPoint cp;
      cp.m_pos = intersect;
      cp.m_tri = tri;
      cutpoints->push_back(cp);
    }
  }

  // Sort by distance from lineseg start
  std::sort(cutpoints->begin(), cutpoints->end(), CutPointSorter(seg.p0));
}

void Squishy::StartCut(const LineSeg& seg, float cutDepth)
{
  m_cutLines.clear();

  float sqDepth = cutDepth * cutDepth;

  CutPoints cutpoints;
  GetCutPoints(seg, &cutpoints); 
 
  // Order the tris by distance. Taking the closest tri as cut depth 0, do we 
  //  reach the next tri? If so, we are cutting through.

  const CutPoint& closest = cutpoints[0];

  for (int i = 0; i < cutpoints.size(); i++)
  {
    CutPoint& cp = cutpoints[i];
 
    if (i > 0)
    {
      // Test distance from cut point 0 to this one
      float sqdist = (closest.m_pos - cp.m_pos).SqLen();
      if (sqdist > sqDepth)
      {
        break;
      }
    }
    // Create a new cutline for this depth
    CutLine cl;
    cl.push_back(cp);
    m_cutLines.push_back(cl);
  }
}

void Squishy::ContinueCut(const LineSeg& seg, float cutDepth)
{
  CutPoints cutpoints;
  GetCutPoints(seg, &cutpoints); 

  int size = std::min(cutpoints.size(), m_cutLines.size());

  for (int i = 0; i < size; i++)
  {
    CutPoint& cp = cutpoints[i];

    CutLine& cl = m_cutLines[i];
    cl.push_back(cp);
  }
}

std::ostream& operator<<(std::ostream& os, const Squishy::Tri& tri)
{
  return os <<
    tri.m_particles[0] << "/" << 
    tri.m_particles[1] << "/" <<
    tri.m_particles[2];
}

void PrintTri(const Squishy::Tri& tri)
{
  std::cout << tri;
}

bool CommonEdge(const Squishy::Tri& tr1, const Squishy::Tri& tr2, int* v1, int* v2)
{
  // Find edge shared by tris
  // Get the number of times each of the 3 tri verts in tr1 appears in the
  //  list of verts for tr2.
  int p1 = std::count(tr2.m_particles, tr2.m_particles + 3, tr1.m_particles[0]);
  int p2 = std::count(tr2.m_particles, tr2.m_particles + 3, tr1.m_particles[1]);
  int p3 = std::count(tr2.m_particles, tr2.m_particles + 3, tr1.m_particles[2]);

  // Number of verts shared between tr1 and tr2 -- should be 2 if they are neighbours
  int num = p1 + p2 + p3; 
std::cout << "Found " << num << " matching verts, "; 
  if (num == 2)
  {
    int e[2] = { 0, 1 };
    if (p1 == 0)
    {
      e[0] = 2;
    }
    else if (p2 == 0)
    {
     e[1] = 2;
    }
   
    *v1 = tr1.m_particles[e[0]];
    *v2 = tr1.m_particles[e[1]];
    // Put in ascending order
    if (*v1 > *v2)
    {
      std::swap(*v1, *v2);
    } 
    std::cout << "Edge " << e[0] << "-" << e[1] << ": " << *v1 << " - " << *v2 << "\n";
    return true;
  }
std::cout << "\n";
  return false;
}

void Squishy::CutInto(const CutLine& cutline)
{
std::cout << "Cutting...\n";

  // Create a new squishy or change the existing data?
  // Changes to make:
  // Add new particles, add new springs
  // Remove some springs ?
  // Remove tris, replace with more as tris tesselated

  // Presumably would not be OK to just take first and last point and make a 
  //  straight cut..
  // But could remove points on the same tri, just ending up with 1 or 2 points per tri?
  // Otherwise, we would be pointing to tris which would no longer exist after being
  //  tesselated.
  // Cases are:
  //  - all points on the same tri - assume we can just cut straight from first to last?

  if (cutline.empty())
  {
    return;
  }

  int size = cutline.size();
  CutPoint start = cutline[0];
  
  for (int i = 1; i < size; i++)
  {
    // Check point i and i+1. Same tri or different?
    const CutPoint& cp = cutline[i];
    if (cp.m_tri == start.m_tri)
    {
      // Same tri. Skip this cut point
      
    }
    else
    {
      // start and cp are on different tris - they should be neighbours sharing an edge ?
      // Find point on edge.
      Tri* tr1 = start.m_tri;
      Tri* tr2 = cp.m_tri;

std::cout << "Finding edge between 2 tris: TR1: " << *tr1 << " TR2: " << *tr2 << "\n";
      // Find common edge (if exists)
      int e1 = 0;
      int e2 = 0;
      if (CommonEdge(*tr1, *tr2, &e1, &e2))
      {
        // Same edge as last time? Skip if so
        // TODO

        // Tesselate start tri
        // Find the point on the edge where the cut line crosses it - use line3 - line3
        //  intersection.
        LineSeg triEdge(GetParticle(e1)->GetPos(), GetParticle(e2)->GetPos());
        LineSeg cut(start.m_pos, cp.m_pos);
        LineSeg closest; // shortest line seg connecting the tri edge and cut segment
        float mua, mub; // t values along segments, not used
        if (Intersects(triEdge, cut, &closest, &mua, &mub))
        {
          std::cout << "Found point on tri edge!\n";
        }
      }

      start = cp;
    }  
  }
}

void Squishy::EndCut(const LineSeg&, float cutDepth)
{
  int numLines = m_cutLines.size();
  // TODO Could be one or more lines
  switch (numLines)
  {
  case 0:
    return;

  case 1:
    // Cutting into the squishy but not all the way through to the back
    CutInto(m_cutLines[0]);

    break;

  case 2:
    // Cutting at least partially all the way through. 
    // This wil make a hole where points along the front line are matched by points
    //  along the line at the back.
    break;

  default:
    // Squishy is folded, and we are cutting through the layers
    break;
  }
}

void Squishy::SetNumVerts(int n)
{
  m_numVerts = n;
}

bool Squishy::Init(const std::string& objFilename, float k)
{
/*
  // TODO TEMP TEST
  // This is to check the file is OK and to give a comparison with the new loading code.
  m_mesh = (ObjMesh*)TheResourceManager::Instance()->GetRes(objFilename);
  if (!m_mesh)
  {
    std::cout << "Failed to load obj mesh!\n";
    return false;
  }
*/

  if (!SquishyLoadObj(this, objFilename))
  {
    return false;
  }

  // Create particles at verts. Connect with springs at triangle edges.

  // TODO Load in obj mesh, retaining index format - ObjMesh class converts to
  //  tri list.
/*
  typedef std::map<Vec3f, int, Vec3Comp> Verts;
  Verts verts;

  typedef std::pair<int, int> Edge;
  typedef std::set<Edge> Edges;
  Edges edges;

  // Use collision mesh to get tri data - TEMP
  CollisionMesh cm;
  mesh->CalcCollisionMesh(&cm);
  const CollisionMesh::Tris& tris = cm.GetAllTris();
//  for (const Tri& tri : tris)
  for (auto it = tris.begin(); it != tris.end(); ++it)
  {
    const Tri& tri = *it;

    int prevParticle = -1;
    for (int i = 0; i < 3; i++)
    {
      const Vec3f& v = tri.m_verts[i];

      int id = -1;
      // Look up v - if we've already found it, we have a particle already
      auto iter = verts.find(v);
      if (iter == verts.end())
      {
        // No particle created yet
        id = CreateParticle();
        Particle* p = GetParticle(id); // OK this is a bit crap
        p->SetPos(v);
        // Now what about (inv) mass of this particle?
        p->SetInvMass(1.0f); // TODO
      }
      else
      {
        std::cout << "Vert " << iter->second << " found in map.\n";
        id = iter->second;
      }
     
      // Create edge
      if (i > 0)
      {
        //edges.insert(Edge(id, prevParticle));
      }
      prevParticle = id;
    }
  }

  for (auto it = edges.begin(); it != edges.end(); ++it)
  {
    const Edge& e = *it;
    int id = CreateSpring(e.first, e.second);
    Spring* spring = GetSpring(id);
    // TODO k, length, etc.
  }
*/

  // Create immovable particles at verts. Connect each vert to its immovable 
  //  particle with spring. Set k value to k given as parameter.

  return true; // TODO needed?
}

void Squishy::Update()
{
  // TODO do nothing if inert

  SpringSystem::Update();

  // Recalc normals
  m_normals.clear();
  m_normals.resize(m_numVerts);

  for (auto it = m_trilist.begin(); it != m_trilist.end(); ++it)
  {
    Tri* tri = *it;

    Particle* particle[3] = 
    {
      GetParticle(tri->m_particles[0]),
      GetParticle(tri->m_particles[1]),
      GetParticle(tri->m_particles[2])
    };
    Vec3f pos[3] = 
    {
      particle[0]->GetPos(),
      particle[1]->GetPos(),
      particle[2]->GetPos()
    };
    Vec3f v1 = pos[1] - pos[0];
    Vec3f v2 = pos[2] - pos[0];
    Vec3f n = CrossProduct(v1, v2);
    n.Normalise();
    tri->m_normal = n;
  
    // Smooth normals
    Assert(tri->m_particles[0] < m_numVerts);
    Assert(tri->m_particles[1] < m_numVerts);
    Assert(tri->m_particles[2] < m_numVerts);

    m_normals[tri->m_particles[0]] += n; 
    m_normals[tri->m_particles[1]] += n; 
    m_normals[tri->m_particles[2]] += n; 
  }
  for (int i = 0; i < m_numVerts; i++)
  {
    if (m_normals[i].SqLen() > 0)
    {
      m_normals[i].Normalise();
    }
  }
}

void Squishy::Draw()
{
  if (m_drawSpringSystem)
  {
    SpringSystem::Draw();
  }

  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
  AmjuGL::Enable(AmjuGL::AMJU_BLEND);
  PushColour();
  MultColour(Colour(1, 1, 1, 0.8f));
//  m_mesh->Draw();

  // TODO TEMP TEST 
  glBegin(GL_TRIANGLES);

  for (auto it = m_trilist.begin(); it != m_trilist.end(); ++it)
  {
    Tri* tri = *it;

    Particle* particle[3] = 
    {
      GetParticle(tri->m_particles[0]),
      GetParticle(tri->m_particles[1]),
      GetParticle(tri->m_particles[2])
    };
    Vec3f pos[3] = 
    {
      particle[0]->GetPos(),
      particle[1]->GetPos(),
      particle[2]->GetPos()
    };
    Vec3f normal[3] = 
    {
      m_normals[tri->m_particles[0]],
      m_normals[tri->m_particles[1]],
      m_normals[tri->m_particles[2]],
    };

    Colour col(1, 1, 1, 1);
    if (tri->m_selected)
    {
      col = Colour(1, 0, 0, 1);
    }
    PushColour();
    MultColour(col);

    glNormal3f(normal[0].x, normal[0].y, normal[0].z);
    glVertex3f(pos[0].x, pos[0].y, pos[0].z);

    glNormal3f(normal[1].x, normal[1].y, normal[1].z);
    glVertex3f(pos[1].x, pos[1].y, pos[1].z);

    glNormal3f(normal[2].x, normal[2].y, normal[2].z);
    glVertex3f(pos[2].x, pos[2].y, pos[2].z);

    PopColour();
  }

  glEnd(); 

  PopColour();
  AmjuGL::Disable(AmjuGL::AMJU_BLEND);
  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);
  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ);

  glLineWidth(3);
  glPointSize(3);
  PushColour();
  MultColour(Colour(0, 1, 0, 1));
  for (int i = 0; i < m_cutLines.size(); i++)
  {
    glBegin(GL_POINTS);
    CutLine& cl = m_cutLines[i];
    for (int j = 0; j < cl.size(); j++)
    {
      CutPoint& cp = cl[j];
      glVertex3f(cp.m_pos.x, cp.m_pos.y, cp.m_pos.z);
    } 
    glEnd();  
  }
  PopColour();
}

void Squishy::AddForce(const Vec3f& pos, const Vec3f& dir)
{
}
  
void Squishy::AddTri(const Tri& tri)
{
  m_trilist.push_back(new Tri(tri));
}

}

