/*
Amju Games source code (c) Copyright Jason Colman 2000-2008
*/

#include "AnimManager.h"
#include "File.h"

namespace Amju
{
void AnimManager::Clear()
{
  m_meshes.clear();
  m_skels.clear();
  m_anims.clear();
}

bool AnimManager::SetUpAnimPlayer(
  AnimPlayer* pPlayer,
  const std::string& meshName,
  const std::string& skinName,
  const std::string& skelName)
{
  PMesh pMesh;
  PSkin pSkin;
  PSkeleton pSkel;

  // Look up/load skeleton
  SkelMap::iterator skelIt = m_skels.find(skelName);
  if (skelIt == m_skels.end())
  {
    pSkel = new Skeleton;
    File skelFile;
    if (!skelFile.OpenRead(skelName)) 
    {
      skelFile.ReportError("Couldn't open skeleton file");
      return false;
    }
    if (!pSkel->Load(&skelFile))
    {
      skelFile.ReportError("Couldn't load skeleton file");
      return false;
    }

    m_skels[skelName] = pSkel;
  }
  else
  {
    pSkel = skelIt->second;
  }

  // Look up mesh

  MeshMap::iterator meshIt = m_meshes.find(meshName);
  if (meshIt == m_meshes.end())
  {
    pMesh = new IndexedMesh;
    File meshFile;
    if (!meshFile.OpenRead(meshName))
    {
      meshFile.ReportError("Couldn't open mesh file");
      return false;
    }
    if (!pMesh->Load(&meshFile))
    {
      meshFile.ReportError("Failed to load mesh file");
      return false;
    }
    
    pSkin = new Skin;
    pSkin->SetSkeleton(pSkel);
    File skinFile;
    if (!skinFile.OpenRead(skinName))
    {
      skinFile.ReportError("Couldn't open skin file");
      return false;
    }
    if (!pSkin->Load(&skinFile))
    {
      skinFile.ReportError("Couldn't load skin file");
      return false;
    }
    pMesh->SetSkin(pSkin);

    m_meshes[meshName] = MeshPair(pMesh, pSkin);
  }
  else
  {
    // TODO Check that same skin is always used with a given mesh
    pMesh = meshIt->second.first;
    pSkin = meshIt->second.second;
  }
  
  pPlayer->CreateBones(pSkel.GetPtr());
  pPlayer->SetMesh(pMesh);
    
  return true;
}

bool AnimManager::LoadAnim(const std::string& animName, const std::string& skelName)
{
  if (m_anims.find(animName) != m_anims.end()) 
  {
    return true;
  }
  PAnim pAnim = new Anim;

  PSkeleton pSkel = m_skels[skelName];
  pAnim->SetSkeleton(pSkel);

  File f;
  if (!f.OpenRead(animName))
  {
    f.ReportError("Couldn't open anim file");
    return false;
  }
  if (!pAnim->Load(&f))
  {
    return false;
  }
  m_anims[animName] = pAnim;
  return true;
}

PAnim AnimManager::GetAnim(const std::string& animName)
{
  Assert(m_anims.find(animName) != m_anims.end());
  return m_anims[animName];
}
}

