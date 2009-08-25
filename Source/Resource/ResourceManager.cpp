#include "ResourceManager.h"
#include "Md2Model.h"
#include "Texture.h"
#include "File.h"
#include "ReportError.h"
#include "StringUtils.h"

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
  File f(false, File::STD);
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

Resource* ResourceManager::ResGroup::GetRes(const std::string& resName)
{
  ResMap::iterator it = m_map.find(resName);
  if (it == m_map.end())
  {
    return 0;
  }
  return it->second;
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

  ResGroup rg;
  rg.m_name = rgName;
  File f;
  if (!f.OpenRead(rgName + ".txt"))
  {
    ReportError("Failed to load resource group " + rgName);
    return false;
  }

  // Add group to map now so it exists if a resource needs it 
  m_groups[rgName] = rg; 

  ResGroup* rgRef = &m_groups[rgName];

  std::string s;
  while (f.GetDataLine(&s))
  {
    PResource res = LoadRes(s);
    rgRef->m_map[s] = res;
  }

  return true;
}

void ResourceManager::FreeResourceGroup(const std::string& rg)
{
  ResGroupMap::iterator it = m_groups.find(rg);
  Assert(it != m_groups.end());
  m_groups.erase(it);
}

/*
Resource* ResourceManager::GetRes(const std::string& resName, Loader loader)
{
  ResMap::iterator it = m_map.find(resName);
  if (it != m_map.end())
  {
    return it->second.GetPtr();
  }

  Resource* r = loader(resName);

  m_map[resName] = r;
  return r;
}
*/

Resource* ResourceManager::GetRes(const std::string& resName)
{
  for (ResGroupMap::iterator it = m_groups.begin();
       it != m_groups.end();
       ++it)
  {
    Resource* res = it->second.GetRes(resName);
    if (res)
    {
      return res;
    }
  }
#ifdef _DEBUG
  ReportError("Couldn't find resource " + resName + " in any group");
#endif
  return 0;
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
}
