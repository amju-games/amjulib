#pragma once

#include <TriList.h>
#include <ObjMesh.h>
#include <Capsule.h>
#include "SpringSystem.h"

namespace Amju
{
// When we create the initial shape from a mesh, each vertex (particle) gets a spring from its bind pose position, which is
//  immovable, to its current position. The k value for these springs determines how squishy or tough the squishy is.
// When we push a vertex, a force due to the spring acts to move back against the push.
// When a vert moves, we should act to retain the volume of the shape overall.
// Step 1 is to work out the volume of the initial shape - must be closed?
// 
class Squishy : public SpringSystem
{
public:
  Squishy();

  // Make from an .obj mesh 
  // TODO Should this be some other kind of mesh, e.g. collision mesh?
  // k is spring k value, which determines the squishiness
  bool Init(const std::string& objFilename, float k);

  // Poke the squishy at some position with the given force
  // NB Could also be pulling, e.g. clamping?
  void AddForce(const Vec3f& pos, const Vec3f& dir);

  // TODO
  // 'Pin'/freeze a point on the squishy so it can not move - i.e. inverse mass = 0
  //  (Find closest particle to the given position?)
//  void PinVertex(const Vec3f& pos);

  // Select all verts in the given capsule
  void SelectVerts(const Capsule& cap);
  void ClearSelection();
  // ??? Or apply force???
  void MoveSelectedVerts(const Vec3f& move);
  // TODO Pin/release selection?
  // ReleaseAll?

  void Draw() override;

  void Update() override;

  // Represents triangle in the squishy.
  // We want to be able to select tris, exert forces on verts,
  //  and cut tris.
  struct Tri : public RefCounted
  {
    Tri()
    {
      m_particles[0] = -1;
      m_particles[1] = -1;
      m_particles[2] = -1;
      m_selected = false;
    }

    Tri(int p1, int p2, int p3)
    {
      m_particles[0] = p1;
      m_particles[1] = p2;
      m_particles[2] = p3;
      m_selected = false;
    }

    // Vertices of this tri are the 3 particles.
    // These ints are the indexes into m_particles
    int m_particles[3];

    Vec3f m_normal; // ? Recalc when particles move

    // TODO Also point to vertices in vertex buffer for updating positions we draw

    // The 3 neighbours of this tri..??
//    RCPtr<Tri> m_neighbours[3];

    bool m_selected;
  };

  void AddTri(const Tri& tri);
  
  // Set the number of verts in the tri mesh
  void SetNumVerts(int n);

  // Cutting
  // Tesselates triangles, adding more particles and springs. Adds triangles to make
  //  'wall' of cut
  // Line seg is formed by two points along the line given by the mouse position 
  virtual void StartCut(const LineSeg&, float cutDepth);
  virtual void ContinueCut(const LineSeg&, float cutDepth);
  virtual void EndCut(const LineSeg&, float cutDepth);

  struct CutPoint
  {
    Vec3f m_pos;
    Tri* m_tri;
    float m_depth; // distance behind tri we should cut to
  };

protected:
  typedef std::vector<CutPoint> CutPoints;
  // A Cut Line is comprised of points on the mesh surface
  typedef std::vector<CutPoint> CutLine;
  typedef std::vector<CutLine> CutLines;

  void GetCutPoints(const LineSeg& seg, CutPoints* cutpoints);

  void CutInto(const CutLine& cutline);

  // Used internally to fill triangles formed by 3 springs
  void FillTri1(const Vec3f& normal, int centre, int p1, int p2, int e1, int e2);
  void FillTriHoles(const Vec3f& normal, const Squishy::Tri& tr, int e1, int e2, int centre);
  void AddTriWithWinding(const Vec3f& normal, Tri tri);

  // Tesselate quad and add triangles to fill the holes.
  // p1 and p2 should be connected (asserts). p3 and p4 are the other 2 verts of a quad, so they
  // should also be connected (asserts). 
  // BUT we don't know about the other edges. I.e. the quad could be 
  //   p1-p2-p3-p4-p1 
  // OR 
  //   p1-p2-p4-p3-p1.
  void AddQuad(const Vec3f& normal, int p1, int p2, int p3, int p4);

  // Add particle at given pos, returns ID of new particle.
  int AddNewParticle(const Vec3f& pos);

  // Volume of squishy when initialised
  float m_volume;

  // Dynamic because vert positions move when force applied;
  // If cut, new tris are created and we make a new tri list.
  // TODO
//  RCPtr<IndexedTriListDynamic> m_displayTrilist;

  // For each particle, all the tris which share it?

  // Store the Tris making up this squishy
  // TODO Store in an octree or similar for efficient picking etc
  typedef std::set<RCPtr<Tri> > TriList;
  TriList m_trilist;

  // Smoothed normals, per-particle
  typedef std::vector<Vec3f> Normals;
  Normals m_normals;

  // Multiple cut lines if we cut through
  CutLines m_cutLines;

  // Number of verts in mesh: some particles have a corresponding mesh vertex,
  //  but others (centre of mass, buddies) do not. The first m_numVerts particles
  //  should be the ones with visible mesh verts.
  int m_numVerts;
  bool m_drawSpringSystem;
  bool m_drawTris;

  // An edge is a pair of two ints, the IDs of the particles at the endpoints.
  typedef std::pair<int, int> Edge;

  struct EdgePoint
  {
    EdgePoint(const Vec3f& pos, Tri* tri, const Edge& edge) : m_pos(pos), m_tri(tri), m_edge(edge)
    {
    }

    Vec3f m_pos;
    Tri* m_tri;  
    Edge m_edge;
  };
  typedef std::vector<EdgePoint> EdgePoints;
  EdgePoints m_edgePoints;
};


/*
Ray-Tri Intersection
Adapted from http://www.lighthouse3d.com/tutorials/maths/ray-triangle-intersection/
Moller-Haines algorithm
*/
int rayIntersectsTriangle(
  const Vec3f& p, const Vec3f& d, 
  const Vec3f& v0, const Vec3f& v1, const Vec3f& v2,
  Vec3f* intersectionPoint);

std::ostream& operator<<(std::ostream& os, const Vec3f& v);

std::ostream& operator<<(std::ostream& os, const Squishy::Tri& tri);

void PrintTri(const Squishy::Tri& tri);

int ThirdVert(const Squishy::Tri& tr, int e1, int e2);

bool CommonEdge(const Squishy::Tri& tr1, const Squishy::Tri& tr2, int* v1, int* v2);

} // namespace Amju

