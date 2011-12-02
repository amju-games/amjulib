#ifndef TERRAIN_H
#define TERRAIN_H

#include <SceneNode.h>
#include <Line3.h>

namespace Amju
{
// Region has a Terrain. In a tree structure for drawing, collisions. Tile based.
// Terrain is a SceneNode, as it is drawn, but also a Resource..?
// No, it loads resources though, i.e. textures. 
class Terrain : public SceneNode
{
public:
  Vec3f GetMousePos(const LineSeg& mouseLine);

  virtual void Draw();

  // Load() ..??

protected:
  // (Octree of) triangles ??
  // Use Obj Mesh, so we can have different materials for different parts of the terrain..?

  // If a regular grid, we don't need anything fancy, you can just divide the coord to get the triangle.
};

// Get the Terrain (for the current region?)
Terrain* GetTerrain();
}

#endif

