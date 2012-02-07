/*
Amju Games source code (c) Copyright Jason Colman 2009
*/

#include "AmjuFirst.h"
#include <string>
#include "Shadow.h"
#include "ResourceManager.h"
#include "Colour.h"
#include "CollisionMesh.h"
#include "Tri.h"
#include "Plane.h"
#include "File.h"

extern "C"
{
#include "Gpc/gpc.h" // Polygon clipping code - it's in C.
}
#include "AmjuFinal.h"

//#define SHADOW_DEBUG

namespace Amju
{
static const float MAX_HEIGHT = 100.0f;
static const float SMALLEST = 1.0f; // ?
static const float OFFSET = 5.0f; // ?
static const float HEIGHT_RANGE_DOWN = 100.0f;
static const float HEIGHT_RANGE_UP = 100.0f;

void Shadow::Polygon::Tesselate()
{
  AMJU_CALL_STACK;

  int numVerts = m_verts.size();
  const AmjuGL::Vert firstVert = m_verts[0];
  for (int i = 1; i < numVerts - 1; i++)
  {
    AmjuGL::Tri tri;
    tri.m_verts[0] = firstVert;
    tri.m_verts[1] = m_verts[i];
    tri.m_verts[2] = m_verts[i + 1];
    m_tris.push_back(tri);
  }
}

void Shadow::Polygon::Draw()
{
  AMJU_CALL_STACK;

  AmjuGL::SetTextureMode(AmjuGL::AMJU_TEXTURE_REGULAR);
  AmjuGL::DrawTriList(m_tris);

#ifdef SHADOW_DEBUG
  for (unsigned int i = 0; i < m_tris.size(); i++)
  {
    const AmjuGL::Tri& tri = m_tris[i];
    AmjuGL::Vec3 v0(tri.m_verts[0].m_x, tri.m_verts[0].m_y, tri.m_verts[0].m_z);
    AmjuGL::Vec3 v1(tri.m_verts[1].m_x, tri.m_verts[1].m_y, tri.m_verts[1].m_z);
    AmjuGL::Vec3 v2(tri.m_verts[2].m_x, tri.m_verts[2].m_y, tri.m_verts[2].m_z);

    AmjuGL::DrawLine(v0, v1);
    AmjuGL::DrawLine(v1, v2);
    AmjuGL::DrawLine(v2, v0);
  }
#endif // SHADOW_DEBUG
}

void Shadow::Polygon::AddVertex(const AmjuGL::Vert& v)
{
  AMJU_CALL_STACK;

  m_verts.push_back(v);
}

Shadow::Shadow()
{
  AMJU_CALL_STACK;

  m_mult = 1.0f;

  m_heightRangeUp = HEIGHT_RANGE_UP; 
  m_heightRangeDown = HEIGHT_RANGE_DOWN;

  m_oldx = 1000.0f; // so list is initially built
  m_oldy = 0;
  m_oldz = 0;
  m_oldsize = -1;
  m_forceRefresh = true;

  m_size = 0;

  SetBlended(true);
}

bool Shadow::Load(File* f)
{
  AMJU_CALL_STACK;

  std::string res;
  if (!f->GetDataLine(&res))
  {
    f->ReportError("Expected shadow texture resource name");
    return false;
  }

  // Allows other textures, e.g. for blocks
  m_texture = (Texture*)TheResourceManager::Instance()->GetRes(res); 
  Assert(m_texture);

  if (!f->GetFloat(&m_size))
  {
    f->ReportError("Expected shadow size");
    return false;
  }

  return true;
}

void Shadow::SetSize(float size)
{
  m_size = size;
}

void Shadow::ClearCollisionMeshes()
{
  m_mesh.clear();
}

void Shadow::AddCollisionMesh(CollisionMesh* mesh)
{
  m_mesh.push_back(mesh);

/*
  if (mesh == m_mesh[0])
  {
    return;
  }

  // Lose oldest mesh
  for (int i = MAX_MESHES - 1; i > 0; i--)
  {
    m_mesh[i] = m_mesh[i - 1];
  }
  m_mesh[0] = mesh;
*/
}

void Shadow::Draw()
{
  Assert(m_size > 0);
  Vec3f pos(m_combined[12], m_combined[13], m_combined[14]);

  for (unsigned int i = 0; i < m_mesh.size(); i++)
  {
    Assert(m_mesh[i]);
    MyDraw(pos, m_size, *(m_mesh[i]));
  }
}

void Shadow::Update()
{
}

void Shadow::MyDraw(
  const Vec3f& v,
  float size,
  const CollisionMesh& collMesh)
{
  AMJU_CALL_STACK;

  // Assumes that if we don't move, the surface on which we cast a shadow 
  //  won't have changed either.
  if (v.x == m_oldx && v.y == m_oldy && v.z == m_oldz && size == m_oldsize)
  {
    // No need to recalculate shadow. Just draw the contents of the list.
    DrawList();
    return;
  }

  m_oldx = v.x;
  m_oldy = v.y;
  m_oldz = v.z;
  m_oldsize = size;
  m_forceRefresh = false;

  // Recalculate the shadow, and fill the list. 
  RecalculateList(v.x, v.y, v.z, size, collMesh);
  DrawList();
}

void Shadow::SetHeightRange(float up, float down)
{
  AMJU_CALL_STACK;

  m_heightRangeUp = up; 
  m_heightRangeDown = down;
}

void Shadow::BindTexture()
{ 
  AMJU_CALL_STACK;

  Assert(m_texture);
  m_texture->UseThisTexture();
}

void Shadow::RecalculateVerts()
{
  AMJU_CALL_STACK;

  m_verts.clear();

  const float x1 = m_oldx + m_oldsize * m_mult;
  const float x2 = m_oldx - m_oldsize * m_mult;
  const float z1 = m_oldz + m_oldsize * m_mult;
  const float z2 = m_oldz - m_oldsize * m_mult;

  m_verts.push_back(Vec2f(x1, z1));
  m_verts.push_back(Vec2f(x1, z2));
  m_verts.push_back(Vec2f(x2, z2));
  m_verts.push_back(Vec2f(x2, z1));
}

int Shadow::GetNumShadowVerts()
{
  AMJU_CALL_STACK;

  return m_verts.size();
}

float Shadow::GetShadowX(int index)
{
  AMJU_CALL_STACK;

  Assert(index < GetNumShadowVerts());
  return m_verts[index].x;
}

float Shadow::GetShadowZ(int index)
{
  AMJU_CALL_STACK;

  Assert(index < GetNumShadowVerts());
  return m_verts[index].y;
}

void Shadow::DrawList() 
{
  AMJU_CALL_STACK;

  if (m_list.empty())
  {
#ifdef SHADOW_DEBUG
std::cout << "Shadow list is empty\n";
#endif

   return;
  }

//std::cout << "Drawing shadow: " << m_list.size() << " polys\n";

  // m_mult is between 1 (full size shadow) and 0 (no shadow). We use this to 
  // change the transparency of the shadow: smaller shadows are more
  // translucent.
  // Push the new alpha value, RGB is unchanged.
//  PushColour();
//  MultColour(Colour(1, 1, 1, m_mult));

  BindTexture();

//  AmjuGL::PushAttrib(AmjuGL::AMJU_BLEND);
  AmjuGL::Enable(AmjuGL::AMJU_BLEND);

  // Allow read-, but not write-access to the depth buffer. This overlaps
  // shadows properly.
//  AmjuGL::EnableZWrite(false);

  // Adjust height of shadow with height of camera. This is because, as the 
  // camera gets higher, the distance between shadow and ground needs to 
  // increase to avoid z-fighting.
  AmjuGL::PushMatrix();

  for (ShadowList::iterator it = m_list.begin(); it != m_list.end(); ++it)
  {
    it->Draw();
  }

  AmjuGL::PopMatrix();

//  AmjuGL::EnableZWrite(true);
//  AmjuGL::PopAttrib();

//  PopColour(); // pop off the alpha change.
}

void Shadow::RecalcMult(float y, float h)
{
  AMJU_CALL_STACK;

  m_mult = (1.0f - (y - h) / MAX_HEIGHT); 
}

void Shadow::RecalculateList(
  float x, float y, float z,
  float size,
  const CollisionMesh& collMesh)
{
  AMJU_CALL_STACK;

  m_list.clear();

  // Get the height poly which is the heighest below y.

  float h = 0;
  // Get highest point on coll mesh ?
  // Better if we can avoid doing this. Shadow centre (x, z) mat be off
  //  the mesh!
  /*
  if (!collMesh.GetY(Vec2f(x, z), &h)) // TODO Must be highest
  {
    return;
  }

  if (y < h)
  {
    y = h;
  }

  Assert(y >= h);
  if (y - MAX_HEIGHT >= h) 
  {
    // We are too far above the ground to cast a shadow.
#ifdef SHADOW_DEBUG
std::cout << "SHADOW: TOO HIGH TO CAST SHADOW??\n";
#endif

    return;
  }
  else 
  {
    RecalcMult(y, h);
// TODO TEMP TEST//    size *= m_mult;
  }
  */

  Assert(size > 0);

  // Redo the list of shadow vertices. 
  RecalculateVerts();

  // Get a horizontal bounding square centred on x, z.
  // This is used to get all the heightservers we need to consider.
  // It doesn't imply that the shadow must be a quad.
  const float x1 = x + size;
  const float x2 = x - size;
  const float z1 = z + size;
  const float z2 = z - size;

  // The height at the edges of the shadow may be higher than at
  // the centre. To make sure we always get the correct Height Poly,
  // add an offset to the height.
  // TODO get exact offset using trig. and size.
  float ymax = y + m_heightRangeUp; 
  float ymin = y - m_heightRangeDown;
  CollisionMesh::Tris tris;

  // Get all height polys which this shadow may cover.
  collMesh.GetAllTrisInBox(AABB(x2, x1, ymin, ymax, z2, z1), &tris);

#ifdef SHADOW_DEBUG
  if (tris.empty())
  {
    std::cout << "SHADOW: NO HEIGHT POLYS!!!\n";
  }
#endif

  // Fix for shadows being cast through floors:
  // Only use the height polys with the highest max y value.
  // It's ok if there are several of these. But discard any polys with a
  // lower max y.
  // TODO
  // use std::partition ?

  for (CollisionMesh::Tris::iterator it = tris.begin(); 
       it != tris.end(); 
       ++it)
  {
#ifdef SHADOW_DEBUG
    // Draw the floor poly
    //glLineWidth(4);
    //AmjuGL::Disable(GL_TEXTURE_2D);
    PushColour();
    //MultColour(Colour(0, 0, 0, 1.0f));
    (*it).Draw();
    PopColour();
    //AmjuGL::Enable(GL_TEXTURE_2D);
#endif

    // Now for each height poly, clip the shadow poly to it.
    const Tri& t = *it;
    // Discard if tri is facing down
    Plane p(t);
    if (p.B() > 0.001f)
    {
      ClipShadowToTri(t, x, y, z, size);
    }
  }
#ifdef SHADOW_DEBUG
  if (m_list.empty())
  {
    std::cout << "HEY! Empty shadow list!\n";
  }
#endif
}

// Map 2D point (x, z) to texture coord (s, t).
// Centre of texture map should fall on (centrex, centrez).
// x is between centrex-size and centrex+size.
// z is between centrez-size and centrez+size.
void Shadow::MapST(
  float centrex, 
  float centrez, 
  float x, 
  float z, 
  float size, 
  float* s, 
  float* t)
{
  AMJU_CALL_STACK;

  Assert(size > SMALLEST);

  const float twosize = 2 * size;
  *s = (x - centrex + size) / twosize;
  *t = (z - centrez + size) / twosize;
}

void Shadow::ClipShadowToTri(
  const Tri& tri, 
  float centrex, 
  float y, 
  float centrez,
  float size) 
{
  AMJU_CALL_STACK;

  // Clip the shadow poly to the given height poly (which should be a triangle).

  // One or more vertices of the shadow may fall outside the poly the shadow 
  // is cast on. We have to get the intersection of the two polygons.
  // Luckily we can use the GPC ("Generic Polygon Clipper").

  // Vector of shadow polygon vertices after clipping.
  std::vector<Vec3f> vertices;

  // Set up gpc data structures for the two polygons.
  // First poly: the shadow 
  gpc_vertex v1[36]; // TODO arbitrary max number of verts.
  int numShadowVerts = GetNumShadowVerts();

  int i;
  for (i = 0; i < numShadowVerts; i++)
  {
    v1[i].x = GetShadowX(i);
    v1[i].y = GetShadowZ(i);
  }

  gpc_vertex_list vl1;
  vl1.num_vertices = numShadowVerts;
  vl1.vertex = v1;

  gpc_polygon p1;
  p1.num_contours = 1;
  p1.contour = &vl1;
  int h1 = 0;
  p1.hole = &h1;

  // Second poly: convert hp to gpc data structures.
  const int numverts = 3; // vertices in ground poly
  const int MAX_VERTS = 36; // TODO get real max from scene ?
  gpc_vertex v2[MAX_VERTS]; // Don't allocate with new, for speed.
  int maxverts = numverts;
  if (maxverts > MAX_VERTS)
  {
    maxverts = MAX_VERTS;
  }

  for (i = 0; i < 3; i++)
  {
    v2[i].x = tri.m_verts[i].x;
    v2[i].y = tri.m_verts[i].z;
  }

  gpc_vertex_list vl2;
  vl2.num_vertices = numverts;
  vl2.vertex = v2;

  gpc_polygon p2;
  p2.num_contours = 1;
  p2.contour = &vl2;
  int h2 = 0;
  p2.hole = &h2;

  // Get intersection
  gpc_polygon result;
  gpc_polygon_clip(GPC_INT, &p1, &p2, &result);

  // Convert result into vector of VertexBase.
  if (result.num_contours != 1)
  {
    // Error, there should be one gpc 'contour' (== polygon here).
#ifdef SHADOW_DEBUG
std::cout << "SHADOW: BAILING before cliiping shadow poly to height poly!\n";
#endif
    return;
  }

  gpc_vertex_list& vlist = result.contour[0];
  for (int k = 0; k < vlist.num_vertices; k++)
  {
    gpc_vertex& v = vlist.vertex[k];
    Vec3f vb((float)v.x, 0, (float)v.y);
    vertices.push_back(vb);
  }

  // Free the contour in the polygon result.
  gpc_free_polygon(&result);

  // TODO we could do this in the loop above.
  Polygon poly;
  for (unsigned int j = 0; j < vertices.size(); j++)
  {
    const Vec3f& v = vertices[j];
    float y = 0;
    bool gotY = GetY(tri, Vec2f(v.x, v.z), &y);
    Assert(gotY);
    //Assert(y != HeightServer::BadHeight);

    float s, t;
    MapST(centrex, centrez, v.x, v.z, size, &s, &t);
    //TexVertex texv(s, t);
    //SceneVertex sv(v.x, y + OFFSET, v.z);

    poly.AddVertex(AmjuGL::Vert(v.x, y + OFFSET, v.z, s, t, 0, 1.0f, 0));
    //poly.AddTexVertex(texv);
  }

  // Finished creating the polygon - now tesselate it
  poly.Tesselate();

  //poly.SetPerpendicularNormals(); // bug no 10, create correct normal.
  m_list.push_back(poly);
}

}

