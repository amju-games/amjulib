/*
Amju Games source code (c) Copyright Jason Colman 2000-2008
*/

#ifndef AMJU_BONE_WEIGHT_H_INCLUDED
#define AMJU_BONE_WEIGHT_H_INCLUDED

#include <vector>
#include <utility>

namespace Amju
{
// Every vertex on a mesh has one BoneWeight object.
// We should use vertex indices as we are duplicating skin data now.
// One boneweight object holds an array of (bone index, weight) for up to 4 bones.
class BoneWeight
{
public:
  BoneWeight();

  int GetNumBones() const;

  // boneNum should be 0..number of bones - 1
  int GetBoneIndex(int boneNum) const;

  // Weight should be between 0 and 1.0 ???
  float GetWeight(int boneNum) const;

  void AddBoneWeight(int boneIndex, float weight);

private:
  // Could be a vector, but probably just an array of 4 (Eberly says 4 ok)
  enum { MAX_NUM_BONE_WEIGHTS = 4 };
  typedef std::pair<int, float> BW;
  BW m_bw[MAX_NUM_BONE_WEIGHTS];
  int m_numBones;
};

// We use indexes, not pointers to Bones, so no need for (smart) pointers here..?
// Boneweights vector uses vertex index to look up the BoneWeight for a vertex.
// I.e this vector should have the same size as the number of vertices as the Mesh.
typedef std::vector<BoneWeight> BoneWeights;
}

#endif

