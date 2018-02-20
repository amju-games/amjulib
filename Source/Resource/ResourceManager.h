#ifndef RESOURCE_MANAGER_H_INCLUDED
#define RESOURCE_MANAGER_H_INCLUDED

#include <map>
#include <set>
#include <string>
#include "Singleton.h"
#include "RCPtr.h"

namespace Amju
{
// * Resource *
// An asset loaded from file, e.g. a texture, shader, mesh, etc.
class Resource : public RefCounted
{
public:
  virtual bool Load(const std::string& resName) = 0;

  // Implement reloading for the Resource type.
  // Hopefully this is as simple as Load(m_resourceName) but you might
  //  need to specialise it, especially for GL types.
  virtual void Reload();

  // We will call this when first loading the resource, so subclasses don't
  //  need to do this.
  void SetResName(const std::string& resName);

  // Return res name if loaded successfully, else empty string if not loaded
  //  or set up in code rather than from a file.
  const std::string& GetResName() const;

protected:
  // Store the resource name we used to first load the resource. This is so
  //  we can reload later if required (probably only for development).
  // It's a 'resource name' rather than filename because resource types can
  //  make a different file name from the resource name. E.g. font resource
  //  names end with '.font', but the actual filename ends '.txt'.
  std::string m_resourceName;
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
  void DebugPrint();

  // Clear all cached resources
  void Clear();

  // Reload all resources
  void Reload();

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

