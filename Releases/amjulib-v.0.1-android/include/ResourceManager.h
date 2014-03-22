#ifndef RESOURCE_MANAGER_H_INCLUDED
#define RESOURCE_MANAGER_H_INCLUDED

#include <map>
#include <set>
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
  // Return a value for the 'initialisation trick'
  bool AddLoader(const std::string& fileExt, Loader);

  // Load a Resource Group
  // This is a list of Resources which will be loaded on demand.
  bool LoadResourceGroup(const std::string& resourceGroupFileName);

  // Free up all resources in the named group
  void FreeResourceGroup(const std::string& resourceGroupFileName);

  // Get a resource - the extension is used to decide which Loader 
  //  to use. The named resource may not have been loaded yet -- 
  //  in which case we load it now. 
  Resource* GetRes(const std::string& resName);

  // Dump info
  void Dump();

  // Clear all cached resources
  void Clear();

private:
  Resource* LoadRes(const std::string& resName);

private:
  // Map of extensions to Loader functions
  typedef std::map<std::string, Loader> LoaderMap;
  LoaderMap m_loaders;

  // Map of resource name to resource - may be zero, not loaded until
  //  required (i.e. GetRes called).
  typedef std::map<std::string, PResource> Resources;
  Resources m_resources;

  // Resource Group - has set of resource names.
  struct ResGroup
  {
    std::string m_name;
    typedef std::set<std::string> ResourceNames;
    ResourceNames m_resNames;
  };
  
  // Map of resource group name to resource group
  typedef std::map<std::string, ResGroup> ResGroupMap;
  ResGroupMap m_groups;
};
}

#endif

