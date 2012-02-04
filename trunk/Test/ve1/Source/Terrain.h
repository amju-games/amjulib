#ifndef TERRAIN_H
#define TERRAIN_H

#include "Ve1Object.h"
#include <SceneNode.h>
#include <Line3.h>

namespace Amju
{
class CollisionMesh;

class TerrainSceneNode;

// Region has a Terrain. In a tree structure for drawing, collisions. Tile based.
// Terrain has a SceneNode, as it is drawn, but is itself a game object (allowing it to be downloadable).
class Terrain : public Ve1Object
{
public:
  Terrain();

  // Get location clicked on, for user navigation
  Vec3f GetMousePos(const LineSeg& mouseLine);

  virtual void Draw(); //?
  virtual void Update();
  virtual bool Load(File*);

  static const char* TYPENAME;
  virtual const char* GetTypeName() const;
  // Load() ..??

// Get the Terrain (for the current region?)
  static Terrain* GetTerrain();

  virtual void OnLocationEntry();

  // For collision tests/shadows -- TODO Octree
  CollisionMesh* GetCollisionMesh();

protected:
  std::string m_objFilename;

  // (Octree of) triangles ??
  // Use Obj Mesh, so we can have different materials for different parts of the terrain..?

  // If a regular grid, we don't need anything fancy, you can just divide the coord to get the triangle.

  TerrainSceneNode* m_sceneNode;
};

}

#endif

