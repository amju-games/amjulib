/*
Amju Games source code (c) Copyright Jason Colman 2000-2008
*/

#ifndef AMJU_SKELETON_H_INCLUDED
#define AMJU_SKELETON_H_INCLUDED

// Skeleton for animation:
// This sets up the tree structure of bones. 

#include <map>
#include "SharedPtr.h"
#include "Bone.h"
#include "Matrix.h"

namespace Amju
{
class File;

class Skeleton;
typedef SharedPtr<Skeleton> PSkeleton;

class Skeleton : public Shareable
{
public:
  void CreateBoneArray(Bones* pBones) const;
 
  bool Load(File*);
#ifdef SCENE_EDITOR
  bool Save(File*);
#endif

  int GetNumBones() const;
 
  int GetIdForString(const std::string& name);

  const Matrix& GetInverseCombinedBindPoseTransform(int boneIndex);

private:

  // Traverse bone names tree, combining matrices
  void CombineMatrices(int boneNameIndex);

  // Invert the combined matrices, needed for skinning
  void InvertBindPoseMatrices(int boneNameIndex);

  struct BoneName
  {
    int m_id;

    std::vector<int> m_childIds;
    std::vector<std::string> m_childStrs;

    // Local transform for bind pose bone
    Matrix m_matrix;
    // Combined transform for bind pose
    Matrix m_combined;
    // Inverse of combined transform, used when drawing mesh
    Matrix m_invBPMat;
  };

  typedef std::vector<BoneName> BoneNames;
  BoneNames m_boneNames;

  typedef std::map<std::string, int> IdMap;
  IdMap m_idMap;
};
}

#endif

