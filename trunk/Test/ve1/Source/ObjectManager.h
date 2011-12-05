#ifndef OBJECT_MANAGER_H_INCLUDED
#define OBJECT_MANAGER_H_INCLUDED

#include <Singleton.h>
#include <string>
#include <set>
#include <RCPtr.h>
#include <vector>
#include <map>

namespace Amju
{
// Info about an object we need to create.
struct Object : public RefCounted
{
  int m_id;
  int m_owner;
  std::string m_type;
  std::string m_assetlist;
  std::string m_datafile;
 
  bool m_datafileLocal;
  bool m_assetsLocal;

  Object(int id, int owner, const std::string& type, const std::string& assetlist, const std::string& datafile) :
    m_id(id), m_owner(owner), m_type(type), m_assetlist(assetlist), m_datafile(datafile), 
    m_datafileLocal(false), m_assetsLocal(false)
  {}  

  void Load();
  void GetDatafile();
  void OnDatafileDownloaded();
};

typedef RCPtr<Object> PObject;

inline bool operator<(const PObject& o1, const PObject& o2)
{
  if (o1->m_id < o2->m_id) return true;
  if (o1->m_owner < o2->m_owner) return true;
  if (o1->m_type < o2->m_type) return true;
  if (o1->m_assetlist < o2->m_assetlist) return true;
  return false;
}


struct AssetList : public RefCounted
{
  std::string m_name; 

  // State of asset list, progresses through these stages but can skip forward.
  enum State
  {
    AMJU_AL_UNKNOWN,
    AMJU_AL_DOWNLOADING,
    AMJU_AL_LOADING,
    AMJU_AL_ALL_ASSETS_LOADED
  };

  State m_state;

  // Count of assets which need to be downloaded. Decremented when the file is downloaded.
  int m_numAssetsDownloading;

  typedef std::vector<std::string> AssetNames;
  AssetNames m_assetNames;

  typedef std::vector<Object*> WaitingObjs;
  WaitingObjs m_waitingObjs; // all objects waiting for this asset list to be loaded 

  AssetList(const std::string& name) : 
    m_name(name), m_state(AMJU_AL_UNKNOWN), m_numAssetsDownloading(0)  {}

  // Load contents once file is local
  bool Load();

  // Add a new object which depends on this asset list. 
  // If the asset list is ready, we can load the object immediately, otherwise the object
  //  gets loaded when assets are downloaded.
  void AddObject(Object* obj);

  void Inc(const std::string& assetname);
  void Dec(const std::string& assetname);
};

typedef RCPtr<AssetList> PAssetList;


struct Asset : public RefCounted
{
  std::string m_name;

  enum State
  {
    AMJU_ASSET_UNKNOWN,
    AMJU_ASSET_DOWNLOADING,
    AMJU_ASSET_LOCAL
  };

  State m_state;

  typedef std::vector<AssetList*> WaitingLists;
  WaitingLists m_waitingLists; // all asset lists waiting for this asset to be loaded

  Asset(const std::string& name) : m_name(name), m_state(AMJU_ASSET_UNKNOWN) {}

  // Add a list which depends on this asset.
  void AddList(AssetList*);

  // Notify all waiting asset lists that this asset has downloaded
  void OnDownloaded();
};

typedef RCPtr<Asset> PAsset;

inline bool operator<(const PAsset& a1, const PAsset& a2)
{
  if (a1->m_name < a2->m_name) return true;
  return false;
}


// Periodically checks for new objects (created since last check).
// (If first time this process, timestamp is set so we get all objects.)
// For new objects, download the asset list. Then download any assets not on local disk.
// When all assets are downloaded, create the object locally.
class ObjectManager : public NonCopyable
{
public:
  ObjectManager();

  // Called every frame
  void Update();

  void AddObject(PObject);

  void AddAsset(const std::string& assetname, AssetList*);

private:
  float m_elapsed;

  // Asset list:
  // Objects point to asset lists.
  // Asset lists point to individual assets. We want asset lists to be updated when an asset gets downloaded.
  // When an asset list is completely downloaded we can create all the objects with that asset list.

  typedef std::set<PObject> Objects;
  Objects m_objects; // objects which we have, or are in the process of downloading/creating.

  typedef std::map<std::string, PAssetList> AssetLists;
  AssetLists m_assetLists;

  typedef std::map<std::string, PAsset> Assets;
  Assets m_assets;
};

typedef Singleton<ObjectManager> TheObjectManager;
}

#endif

