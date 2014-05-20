#pragma once

#include <TriList.h>
#include <ObjMesh.h>
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
  void PinVertex(const Vec3f& pos);

  void Draw() override;

protected:
  // Volume of squishy when initialised
  float m_volume;

  // Dynamic because vert positions move when force applied;
  // If cut, new tris are created and we make a new tri list.
  RCPtr<TriListDynamic> m_trilist;


  // TODO TEMP TEST
  ObjMesh* m_mesh;
};
}
