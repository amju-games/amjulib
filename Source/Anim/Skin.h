/*
Amju Games source code (c) Copyright Juliet Colman 2000-2011
*/

#ifndef SKIN_H_INCLUDED
#define SKIN_H_INCLUDED

#include "BoneWeight.h"
#include <RCPtr.h>
#include "Skeleton.h"

namespace Amju
{
// Data for an animated mesh, has the bone weightings for every vertex.
class Skin : public RefCounted
{
public:
  bool Load(File*);
  bool Save(File*);

  void SetSkeleton(PSkeleton pSkel);

  const BoneWeight& GetBoneweight(int vertIndex);

  const Matrix& GetInverseCombinedBindPoseTransform(int boneIndex);

private:
  BoneWeights m_boneweights;
  PSkeleton m_pSkel;
};

typedef RCPtr<Skin> PSkin;
}

#endif
