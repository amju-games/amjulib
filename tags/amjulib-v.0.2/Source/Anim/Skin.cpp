/*
Amju Games source code (c) Copyright Jason Colman 2000-2011
*/

#include <AmjuFirst.h>
#include <math.h> // fabs
#include "Skin.h"
#include "File.h"
#include "StringUtils.h"
#include <AmjuFinal.h>

namespace Amju
{
const BoneWeight& Skin::GetBoneweight(int vertIndex)
{
  Assert(vertIndex < (int)m_boneweights.size());

  return m_boneweights[vertIndex];
}

void Skin::SetSkeleton(PSkeleton pSkel)
{
  m_pSkel = pSkel;
}

const Matrix& Skin::GetInverseCombinedBindPoseTransform(int boneIndex)
{
  return m_pSkel->GetInverseCombinedBindPoseTransform(boneIndex);
}

bool Skin::Load(File* pf)
{
  int numBoneweights = 0;
  if (!pf->GetInteger(&numBoneweights))
  {
    pf->ReportError("Expected num boneweights (same as num verts in mesh)");
    return false;
  }
  m_boneweights.reserve(numBoneweights);

  for (int i = 0; i < numBoneweights; i++)
  {
    BoneWeight bw;

    int numWeights = 0;
    if (!pf->GetInteger(&numWeights))
    {
      pf->ReportError("Expected number of weights for vert " + ToString(i));
      return false;
    }

    float totalWeight = 0;
    for (int j = 0; j < numWeights; j++)
    {
      std::string jointName; // (== bone Name)
      if (!pf->GetDataLine(&jointName))
      {
        pf->ReportError("Expected joint/bone name");
        return false;
      }
      float w = 0;
      if (!pf->GetFloat(&w))
      {
        pf->ReportError("Expected weight");
        return false;
      }
      totalWeight += w;

      int id = m_pSkel->GetIdForString(jointName);
      if (id < 0)
      {
        pf->ReportError("Bad joint name");
        return false;
      }
      bw.AddBoneWeight(id, w);
    }
    if (fabs(totalWeight - 1.0f) > 0.0001)
    {
      pf->ReportError("Bad total for weights for vert " + ToString(i));
      return false;
    }

    m_boneweights.push_back(bw);
  }

  return true;
}

bool Skin::Save(File* pf)
{
  // TODO
  return true;
}
}
