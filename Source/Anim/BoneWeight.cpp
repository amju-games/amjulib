/*
Amju Games source code (c) Copyright Jason Colman 2000-2008
*/

#include <AmjuFirst.h>
#include "AmjuAssert.h"
#include "BoneWeight.h"
#include <AmjuFinal.h>

namespace Amju
{
BoneWeight::BoneWeight()
{
  m_numBones = 0;
}

int BoneWeight::GetNumBones() const
{
  return m_numBones;
}

// boneNum should be 0..number of bones - 1
int BoneWeight::GetBoneIndex(int boneNum) const
{
  return m_bw[boneNum].first;
}

// Weight should be between 0 and 1.0 ???
float BoneWeight::GetWeight(int boneNum) const
{
  return m_bw[boneNum].second;
}

void BoneWeight::AddBoneWeight(int boneIndex, float weight)
{
  Assert(m_numBones < MAX_NUM_BONE_WEIGHTS);
  m_bw[m_numBones] = std::make_pair(boneIndex, weight);
  m_numBones++;
}

}


