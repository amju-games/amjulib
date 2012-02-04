/*
Amju Games source code (c) Copyright Jason Colman 2009
*/

#ifndef SHADOW_H
#define SHADOW_H

#include "AmjuGL.h"
#include "Vec2.h"
#include "Vec3.h"
#include "Texture.h"
#include "SceneNode.h"

namespace Amju
{
class CollisionMesh;
struct Tri;

// Implement a simple kind of shadow: a dark circle on the ground.
class Shadow : public SceneNode
{
public:
  Shadow();

  // SceneNode overrides
  virtual bool Load(File*);
  virtual void Draw();
  virtual void Update();

  void SetHeightRange(float up, float down);

  void SetSize(float size);

  // Add a CollisionMesh onto which the shadow is cast
  void AddCollisionMesh(CollisionMesh*);
  void ClearCollisionMeshes();

protected:
  void MyDraw(
    const Vec3f& v,
    float size,
    const CollisionMesh& collMesh);

  virtual void BindTexture();

  virtual void RecalculateVerts();

  // Recalculate the list of polys. 
  virtual void RecalculateList(
    float x, 
    float y, 
    float z,
    float size,
    const CollisionMesh& collMesh);

  void ClipShadowToTri(
    const Tri& tri, 
    float x, 
    float y, 
    float z,
    float size);

  // Called by Draw() - draw the polys making up the shadow.
  virtual void DrawList();

  // Map 2D point (x, z) to texture coord (s, t).
  // Centre of texture map should fall on (centrex, centrez).
  // x is between centrex-size and centrex+size.
  // z is between centrez-size and centrez+size.
  virtual void MapST(
    float centrex, 
    float centrez, 
    float x, 
    float z, 
    float size, 
    float* s, 
    float* t);

  // Set m_mult depending on the height of the object above the ground.
  // Should set 0 < m_mult <= 1.0.
  virtual void RecalcMult(float objY, float groundY);

  int GetNumShadowVerts();
  // Get the shadow vertices
  float GetShadowX(int index);
  float GetShadowZ(int index);

protected:
  float m_size;

  // Mesh on which the shadow is cast
  // TODO Get all meshes from the rest of the scene graph ??
  // Allow multiple meshes, so we can continue to cast on a mesh as we leave it and
  //  land on another mesh
  std::vector<CollisionMesh*> m_mesh;

  // Shadow texture - Resource
  PTexture m_texture;

  // Unknown number of verts, as we clip the shadow to the ground poly.
  // But shadow is quad, ground polys are tris, so max verts is 6?
  struct Polygon
  {
    void Draw();
    void AddVertex(const AmjuGL::Vert& v);
    // Call once all vertices have been added
    void Tesselate();
 
    // Tesselated
    AmjuGL::Tris m_tris;

    std::vector<AmjuGL::Vert> m_verts; // 6 max, maybe use array instead ?
 };

  typedef std::vector<Polygon> ShadowList;
  ShadowList m_list;

  // multiplier of size and alpha - depends on height above ground.
  float m_mult; 

  // List of (x, z) shadow verts. 
  // By default this is a simple square.
  // These verts are recalculated in RecalculateVerts().
  std::vector<Vec2f> m_verts;

  float m_heightRangeUp;
  float m_heightRangeDown;

  // Cached position/size and polys, so we only recalculate when necessary.
  float m_oldx, m_oldy, m_oldz, m_oldsize;

  bool m_forceRefresh;
};
}

#endif

