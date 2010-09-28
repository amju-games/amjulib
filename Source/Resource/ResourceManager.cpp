#include "ResourceManager.h"
#include "Md2Model.h"
#include "Texture.h"
#include "File.h"
#include "ReportError.h"
#include "StringUtils.h"
#include <iostream>

//#define RESOURCE_GROUP_DEBUG

namespace Amju
{
// Loader for bmp files
Resource* BmpLoader(const std::string& resName)
{
  Texture* pTex = new Texture;
  if (!pTex->Load(resName))
  {
    return 0;
  }
  return (Resource*)pTex;
}

// Loader for md2 files
Resource* Md2Loader(const std::string& resName)
{
  // MD2 model file
  File f(false);
  if (!f.OpenRead(resName, true))
  {
    ReportError("Failed to open md2 file :-(\n");
    return 0;
  }

  Md2Model* pModel = new Md2Model; // all resources must be on heap
  if (!pModel->Load(&f))
  {
    ReportError("MD2 Load failed :-(\n");
    return 0;
  }
  return (Resource*)pModel;
}

ResourceManager::ResourceManager()
{
  AddLoader("bmp", BmpLoader);
  AddLoader("md2", Md2Loader);
}

bool ResourceManager::AddLoader(const std::string& fileExt, Loader loader)
{
  m_loaders[fileExt] = loader;
  return true;
}

bool ResourceManager::LoadResourceGroup(const std::string& rgName)
{
  if (m_groups.find(rgName) != m_groups.end())
  {
    // Already loaded
    return true;
  }

#ifdef RESOURCE_GROUP_DEBUG
  std::cout << "Loading resource group " << rgName << "... ";
#endif // RESOURCE_GROUP_DEBUG

  ResGroup rg;
  rg.m_name = rgName;
  File f;
  if (!f.OpenRead(rgName + ".txt"))
  {
    ReportError("Failed to load resource group " + rgName);
    return false;
  }

  m_groups[rgName] = rg; 

  ResGroup* rgRef = &m_groups[rgName];

  std::string s;
  while (f.GetDataLine(&s))
  {
    rgRef->m_resNames.insert(s);
  }
 
#ifdef RESOURCE_GROUP_DEBUG
  std::cout << "done\n"; // TODO Time
#endif

  return true;
}

void ResourceManager::FreeResourceGroup(const std::string& rg)
{
  ResGroupMap::iterator it = m_groups.find(rg);
  Assert(it != m_groups.end());
  m_groups.erase(it);

  // TODO Could now purge resources which are not referenced by any group
  //  -- have a separate Purge() function
}

Resource* ResourceManager::GetRes(const std::string& resName)
{
  // Only check if resource is in a group in debug builds
#ifdef RES_DEBUG
  bool found = false;
  for (ResGroupMap::iterator it = m_groups.begin();
       it != m_groups.end();
       ++it)
  {
    ResGroup& rg = it->second;
    ResGroup::ResourceNames::iterator jt = rg.m_resNames.find(resName);
    if (jt != rg.m_resNames.end())
    {
      found = true;
      break;
    }
  }
  if (!found)
  {
    ReportError("Couldn't find resource " + resName + " in any group");
    Assert(0);
  }
#endif

  PResource res = m_resources[resName];
  if (res == 0)
  {
    // Not loaded yet, so load now
    res = LoadRes(resName);
    m_resources[resName] = res;
  }
  return res;
}


Resource* ResourceManager::LoadRes(const std::string& resName)
{
  // Get file extension, no dot
  std::string ext = GetFileExt(resName);
  LoaderMap::iterator jt = m_loaders.find(ext);
  if (jt == m_loaders.end())
  {
    std::cout << "Unexpected resource! " << resName << "\n";
  }

  Assert(jt != m_loaders.end());
  Loader myLoader = jt->second;
  Resource* r = myLoader(resName);
  Assert(r);

  return r;
}

void ResourceManager::Dump()
{
  std::cout << "======================================================================"
    "\nResource Manager: Groups: " << m_groups.size() << "\n";

  for (ResGroupMap::iterator it = m_groups.begin();
       it != m_groups.end();
       ++it)
  {
    ResGroup& rg = it->second;
    std::cout << "Group: " << it->first << "\n";
    for (ResGroup::ResourceNames::iterator jt = rg.m_resNames.begin();
         jt != rg.m_resNames.end();
         ++jt)
    {
      PResource& res = m_resources[*jt];
      std::cout << " " << *jt << "......." << res.GetPtr();
      if (res.GetPtr())
      {
        std::cout << " RC: " << res->GetRefCount();
      }
      std::cout << "\n";
    }
  }
}
}
