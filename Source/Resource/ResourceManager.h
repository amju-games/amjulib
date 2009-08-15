#ifndef RESOURCE_MANAGER_H_INCLUDED
#define RESOURCE_MANAGER_H_INCLUDED

#include <map>
#include <string>
#include "Singleton.h"
#include "RCPtr.h"

namespace Amju
{
class Resource : public RefCounted
{
};
typedef RCPtr<Resource> PResource;

class ResourceManager;
typedef Singleton<ResourceManager> TheResourceManager;

class ResourceManager
{
private:
  ResourceManager();
  friend class Singleton<ResourceManager>;

public:
  // Add Resource Loader function for each supported Resource type
  typedef Resource* (*Loader)(const std::string& resName);
  // Add loader for given file extension (no dot), e.g. "bmp"
  // Return a value for the 'static initialisation trick'
  bool AddLoader(const std::string& fileExt, Loader);

  // Get a resource - the extension is used to decide which Loader 
  //  to use
  Resource* GetRes(const std::string& resName);

  // Get a resource, using the specified Loader
  Resource* GetRes(const std::string& resName, Loader loader);

private:
  typedef std::map<std::string, Loader> LoaderMap;
  LoaderMap m_loaders;

  typedef std::map<std::string, PResource> ResMap;
  ResMap m_map;
};
}

#endif

