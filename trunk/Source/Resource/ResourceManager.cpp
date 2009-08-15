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

Resource* ResourceManager::GetRes(const std::string& resName)
{
  ResMap::iterator it = m_map.find(resName);
  if (it != m_map.end())
  {
    return it->second.GetPtr();
  }

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

  m_map[resName] = r;
  return r;
}
}
