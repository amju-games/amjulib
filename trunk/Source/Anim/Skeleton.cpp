/*
Amju Games source code (c) Copyright Jason Colman 2000-2008
*/

#include <AmjuFirst.h>
#include <iostream>
#include "Skeleton.h"
#include "File.h"
#include "StringUtils.h"
#include <AmjuFinal.h>

namespace Amju
{
int Skeleton::GetNumBones() const
{
  return m_boneNames.size();
}

void Skeleton::CreateBoneArray(Bones* pBones) const
{
#ifdef SKEL_DEBUG
std::cout << " In CreateBoneArray... num bones: " << GetNumBones() << "\n";
#endif

  pBones->clear();

  // Create all the bones up front -- this is necessary as we want to
  // get pointers to succeeding elements 
  pBones->resize(GetNumBones());
  Assert(pBones->size() == GetNumBones());

  for (int i = 0; i < GetNumBones(); i++)
  {
#ifdef SKEL_DEBUG
std::cout << "  Creating Bone " << i << "...\n";
#endif

    Bone& b = (*pBones)[i];
    const BoneName& boneName = m_boneNames[i];

    // So bone starts in bind pose transform until a new transform is set. 
    b.SetTransform(boneName.m_matrix);

    int numChildren = boneName.m_childIds.size();
#ifdef SKEL_DEBUG
std::cout << "  This bone has " << numChildren << " children\n";
#endif
 
    for (int j = 0; j < numChildren; j++)
    {
      int childId = boneName.m_childIds[j];

#ifdef SKEL_DEBUG
std::cout << "    Setting ptr to child " << j << " ID: " << boneName.m_childIds[j] << "\n";
#endif

      // Get address of element childId
      Bone* pChildBone = &((*pBones)[childId]);

      b.AddChild(pChildBone);
    }
  }
}

#ifdef SCENE_EDTIOR 
bool Skeleton::Save(File* pf)
{
  // TODO
  return true;
}
#endif

int Skeleton::GetIdForString(const std::string& name)
{
  if (m_idMap.find(name) == m_idMap.end())
  {
    return -1;
  }

  return m_idMap[name];
}

const Matrix& Skeleton::GetInverseCombinedBindPoseTransform(int boneIndex)
{
  const BoneName& bn = m_boneNames[boneIndex];
  return bn.m_invBPMat;
}

void Skeleton::InvertBindPoseMatrices(int boneNameIndex)
{
  BoneName &bn = m_boneNames[boneNameIndex];

  Matrix inv;
  // TODO DEPENDENCY
  if (!bn.m_combined.Inverse(&inv)) 
  {
    // TODO Can't invert this matrix! What to do ?!
    Assert(0);
    inv.SetIdentity();
  }
  bn.m_invBPMat = inv; 

  for (unsigned int i = 0; i < bn.m_childIds.size(); i++)
  {
    int childId = bn.m_childIds[i];
    InvertBindPoseMatrices(childId);
  }
}

void Skeleton::CombineMatrices(int boneNameIndex)
{
  BoneName &bn = m_boneNames[boneNameIndex];
  if (boneNameIndex == 0)
  {
    bn.m_combined = bn.m_matrix;
  }

  for (unsigned int i = 0; i < bn.m_childIds.size(); i++)
  {
    int childId = bn.m_childIds[i];
    BoneName& child = m_boneNames[childId];
    child.m_combined = child.m_matrix * bn.m_combined;
    CombineMatrices(childId);
  }
}

bool Skeleton::Load(File* pf)
{
#ifdef SKEL_DEBUG
std::cout << "Loading skel...\n";
#endif

  // Get number of bone(name)s
  int numBones = 0;
  if (!pf->GetInteger(&numBones))
  {
    pf->ReportError("Expected num bones");
    return false;
  }

#ifdef SKEL_DEBUG
std::cout << "Skeleton has " << numBones << " bones\n";
#endif

  // For each bone name...
  int i;
  for (i = 0; i < numBones; i++)
  {
    std::string boneStr;
    if (!pf->GetDataLine(&boneStr))
    {
      pf->ReportError("Expected bone (joint) name string");
      return false;
    }

    // Map joint name to position in vector
    // Check name has not already been loaded
    if (m_idMap.find(boneStr) != m_idMap.end())
    {
      pf->ReportError("Duplicated joint name: " + boneStr);
      return false;
    }
    m_idMap[boneStr] = i;

#ifdef SKEL_DEBUG
std::cout << "Joint name: " << boneStr << "\n";
#endif

    BoneName bn;

    // Get bind pose matrix
    if (!bn.m_matrix.Load(pf))
    {
      pf->ReportError("Failed to load transform matrix for bone " + boneStr);
      return false;
    }

    // Get the number of children for this bone
    int numChildren = 0;
    if (!pf->GetInteger(&numChildren))
    {
      pf->ReportError("Expected number of children for bone index" + ToString(i));
    }

#ifdef SKEL_DEBUG
std::cout << " Num children: " << numChildren << ": ";
#endif

    // For each child...
    for (int j = 0; j < numChildren; j++)
    {
      // Get the child name
      std::string childStr;

      if (!pf->GetDataLine(&childStr))
      {
        pf->ReportError("Expected child id " + ToString(j) + " for bone " + boneStr);
      }

#ifdef SKEL_DEBUG
std::cout << childStr << " ";
#endif

      bn.m_childStrs.push_back(childStr);
    }
    m_boneNames.push_back(bn);

#ifdef SKEL_DEBUG
std::cout << "\n";
#endif
  }

  // Look up numbers for names
  for (unsigned i = 0; i < m_boneNames.size(); i++)
  {
    BoneName& bn = m_boneNames[i];
    for (unsigned int j = 0; j < bn.m_childStrs.size(); j++)
    {
      int id = m_idMap[bn.m_childStrs[j]];
      bn.m_childIds.push_back(id);
    }
  }

  // Traverse tree, combining bind pose matrices
  CombineMatrices(0);

  // Invert the bind pose matrices - that's what we need when we tranform
  //  the mesh.
  InvertBindPoseMatrices(0);

  return true;
}
}

