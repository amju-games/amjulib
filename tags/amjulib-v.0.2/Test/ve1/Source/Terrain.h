#ifndef TERRAIN_H
#define TERRAIN_H

#include <SceneNode.h>
#include <Line3.h>
#include "Ve1Object.h"
#include "HasCollisionMesh.h"

namespace Amju
{
class CollisionMesh;

// Location has a Terrain. In a tree structure for drawing, collisions. 
// Terrain has a SceneNode, as it is drawn, but is itself a game object (allowing it to be downloadable).
class Terrain : public Ve1Object, public HasCollisionMesh
{
public:
  Terrain();

  // Get location clicked on, for user navigation. Returns false if not a point on ground
//  bool GetMousePos(const LineSeg& mouseLine, Vec3f* pos);

////  virtual void Draw(); //?
  virtual void Update();
  virtual bool Load(File*);

  static const char* TYPENAME;
  virtual const char* GetTypeName() const;

  virtual void OnLocationEntry();

  virtual CollisionMesh* GetCollisionMesh();

  void SetMeshFilename(const std::string& filename) { m_objFilename = filename; }

  virtual void SetEditMenu(GuiMenu*);

protected:
  std::string m_objFilename;

  // (Octree of) triangles ??
  // Use Obj Mesh, so we can have different materials for different parts of the terrain..?

  // If a regular grid, we don't need anything fancy, you can just divide the coord to get the triangle.

//  SceneCollisionMesh* m_sceneNode;
};

// Get the Terrain for the current location for this client
Terrain* GetTerrain();
void ClearTerrain(); // when we leave current location
bool TerrainReady(); // returns true when a Terrain exists (i.e. GetTerrain() is valid)

}

#endif

