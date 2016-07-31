#ifndef OBJ_USEFUL_FUNCTIONS_H
#define OBJ_USEFUL_FUNCTIONS_H

#include <map>
#include <string>
#include <vector>
#include "Vec3.h"
#include "Vec2.h"
#include <AABB.h>
#include <AmjuGL.h>
#include <TriList.h>
#include <Texture.h>
#include "Material.h"

namespace Amju
{
// Useful types
// ------------

// Face type - assumes all faces are triangles,
//  so is most efficient.
struct Face
{
  Face();

  // only triangles are allowed!
  int m_pointIndex[3];
  int m_normalIndex[3];
  int m_uvIndex[3];
};

typedef std::vector<Face> Faces;
typedef std::map<std::string, Faces> FaceMap;

// Group - has name, material name and collection of faces.
struct Group
{
  Group() : m_visible(true), m_collidable(true) {}

  bool IsVisible() const { return m_visible; }
  bool IsCollidable() const { return m_collidable; }
  void SetVisible(bool visible) { m_visible = visible; }
  void SetCollidable(bool collidable) { m_collidable = collidable; }

  bool m_visible;
  bool m_collidable;
  std::string m_name;
  std::string m_materialName; // TODO int index
  AmjuGL::Tris m_tris; // argh, we need this to make collision mesh
  RCPtr<TriListStatic> m_triList;
  AABB m_aabb;
};

// Group with extra info for saving
struct SaveGroup
{
  Faces m_faces;
};

// Map of group name to group object
typedef std::map<std::string, Group> Groups;

// Other useful typedefs for collections
typedef std::vector<Vec3f> Vecs;
typedef std::vector<Vec2f> UVs;
typedef std::vector<std::string> Strings;


// Useful functions
// ----------------

// Convert a vector of four strings to a Vec3. 
// The zeroth string is ignored. Strings 1, 2 & 3 are
//  asssumed to be floats.
Vec3f ToVec3(const Strings& strs);

// Convert a vector of three strings to a Vec2. 
// The zeroth string is ignored. Strings 1 & 2 are
//  asssumed to be floats.
Vec2f ToVec2(const Strings& strs);

// Convert a vector of strings to a Face.
// The zeroth string is ignored. We only use the first 
//  three vertices.
Face ToFace(const Strings& fstrs);

} // namespace

#endif

