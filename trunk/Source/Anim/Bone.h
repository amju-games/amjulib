/*
Amju Games source code (c) Copyright Jason Colman 2000-2008
*/

#ifndef AMJU_BONE_H_INCLUDED
#define AMJU_BONE_H_INCLUDED

// Bones for animation
// Bones are in a tree structure - each Bone has a list of children.
// Bones are set up by a Skeleton, but the Skeleton is the static structure;
//  the bones tree is the set of transformations which change every frame.
// A bone tree is owned by an AnimPlayer.

#include <string>
#include <vector>
#include "SharedPtr.h"
#include "Matrix.h"
#include "Vec3.h"

namespace Amju
{
class File;

/*
// TODO TEMP TEST
struct Vec3
{
  Vec3(float x = 0, float y = 0, float z = 0) : m_x(x), m_y(y), m_z(z) { } 
  float m_x, m_y, m_z;

  bool Load(File*);
#ifdef SCENE_EDITOR
  bool Save(File*);
#endif
};

Vec3 operator+(const Vec3& w, const Vec3& v);

Vec3 operator-(const Vec3& w, const Vec3& v);

Vec3 operator*(const Vec3& v, float f);
*/

class Bone;
typedef std::vector<Bone> Bones;

class Bone : public Shareable
{
public:
  Bone();

#ifdef _DEBUG
  void Draw(Bone* pParent, int depth) const;
#endif

  void AddChild(Bone* child);

  // Set the interpolated transform for this bone - this is in bone space
  void SetTransform(const Matrix& m);

  // Recursively multiply child transforms, so all bones update their
  // Combined matrix - this is in Skeleton/Character/Mesh space
  void CombineTransforms(Bone* parent);

  const Matrix& GetCombinedTransform() const;

private:
  // Each bone has a set of child bones
  std::vector<Bone*> m_children;

  // Current (interpolated) RST turned into a matrix
  Matrix m_transform;

  // Product of all ancestor transformations and this one
  Matrix m_combined; 
};
}
#endif

