#include <AmjuFirst.h>
#include "ResourceManager.h"
#include "Md2Model.h"
#include "Texture.h"
#include "File.h"
#include "ReportError.h"
#include "StringUtils.h"
#include <Font.h>
#include <iostream>
#include <AmjuFinal.h>

namespace Amju
{
Resource* ShaderLoader(const std::string& resName)
{
  // Remove ".shader" from name, as we append "-vert" and "-frag" etc
  return (Resource*)AmjuGL::LoadShader(GetFileNoExt(resName));
}

// Loader for image files
Resource* ImageLoader(const std::string& resName)
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

void Resource::Reload()
{
  bool res = Load(m_resourceName);
  Assert(res); // should succeed, assuming the original Load did so.
}

void Resource::SetResName(const std::string& resName)
{
  m_resourceName = resName;
}

const std::string& Resource::GetResName() const
{
  return m_resourceName;
}

ResourceManager::ResourceManager()
{
  AddLoader("tga", ImageLoader);
  AddLoader("png", ImageLoader);
  AddLoader("bmp", ImageLoader);
  AddLoader("md2", Md2Loader);
  AddLoader("font", FontLoader);
  AddLoader("shader", ShaderLoader);
}

bool ResourceManager::AddLoader(const std::string& fileExt, Loader loader)
{
  m_loaders[fileExt] = loader;
  return true;
}

Resource* ResourceManager::GetRes(const std::string& resName)
{
  PResource res;
  auto it = m_resources.find(resName);
  if (it == m_resources.end())
  {
    // Not loaded yet, so load now
    res = LoadRes(resName);
    m_resources[resName] = res;
  }
  else
  {
    res = it->second;
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
    Assert(0);
    return 0;
  }

  Assert(jt != m_loaders.end());
  Loader myLoader = jt->second;

  std::cout << "Loading resource: '" << resName << "'.\n";

  Resource* r = myLoader(resName);
  //Assert(r);
#ifdef _DEBUG
  if (!r)
  {
    std::cout << "Failed to load resource: " << resName << "\n";
  }
#endif

  r->SetResName(resName); // for reloading
  return r;
}

void ResourceManager::Reload()
{
  std::cout << "Reloading resources...";
  for (auto& p : m_resources)
  {
    std::cout << p.first << "\n";
    p.second->Reload();
  }
  std::cout << "...Reloaded resources.\n";
}

void ResourceManager::DebugPrint()
{
  std::cout << "Resources:\n";
  for (auto& p : m_resources)
  {
    std::cout << p.first << "\n";
    // TODO Would be good to print size etc
  }
  std::cout << "End of resources list.\n";
}

void ResourceManager::Clear()
{
  m_resources.clear();
}
}
