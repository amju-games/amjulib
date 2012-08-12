#ifndef OBJECT_MANAGER_H_INCLUDED
#define OBJECT_MANAGER_H_INCLUDED

#include <Singleton.h>
#include <string>
#include <set>
#include <RCPtr.h>
#include <vector>
#include <map>
#include "DownloadManager.h"
#include <GameObject.h>

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
 
  bool m_loaded;

  Object(int id, int owner, const std::string& type, const std::string& assetlist, const std::string& datafile) :
    m_id(id), m_owner(owner), m_type(type), m_assetlist(assetlist), m_datafile(datafile), 
    //m_datafileLocal(false), m_assetsLocal(false),
    m_loaded(false)
  { } 

  Object() : m_id(-1), m_owner(-1), m_loaded(false) { } 

  // Create object, load it, add to game (if in current location, else just hold onto it)
  void Create();

  // Load/Save the members of this Object, for caching
  bool Load(File*);
  bool Save(File*);
};

typedef RCPtr<Object> PObject;

inline bool operator<(const PObject& o1, const PObject& o2)
{
  if (o1->m_id < o2->m_id)
  {
    return true;
  }
  return false;

  // The other data doesn't matter: if the IDs are the same, it's the same object.
/*
  else if (o1->m_id > o2->m_id)
  {
    return false;
  }

  if (o1->m_owner < o2->m_owner)
  {
    return true;
  }
  else if (o1->m_owner > o2->m_owner)
  {
    return false;
  }

  if (o1->m_type < o2->m_type)
  {
    return true;
  }
  else if (o1->m_type > o2->m_type)
  {
    return false;
  }

  if (o1->m_assetlist < o2->m_assetlist)
  {
    return true;
  }
  else if (o1->m_assetlist > o2->m_assetlist)
  {
    return false;
  }

  return false;
*/
}


struct AssetList : public RefCounted
{
  std::string m_name; 

  // State of asset list, progresses through these stages but can skip forward.
  enum State
  {
    AMJU_AL_UNKNOWN,
    AMJU_AL_LOADING,
    AMJU_AL_ALL_ASSETS_LOADED
  };

  State m_state;

  void Update();

  bool AllAssetsLoaded() const { return m_state == AMJU_AL_ALL_ASSETS_LOADED; }

  typedef std::vector<std::string> AssetNames;
  AssetNames m_assetNames;

  AssetList(const std::string& name) : 
    m_name(name), m_state(AMJU_AL_UNKNOWN) {}

  // Load contents once file is local
  bool Load();
};

typedef RCPtr<AssetList> PAssetList;


// Periodically checks for new objects (created since last check).
// (If first time this process, timestamp is set so we get all objects.)
// For new objects, download the asset list. Then download any assets not on local disk.
// When all assets are downloaded, create the object locally.
// Looks after all game objects; only GOs in current room are stored in Game.
class ObjectManager : public DownloadManager
{
public:
  ObjectManager();
  ~ObjectManager();

  // Called every frame
  void Update();

  void AddObject(PObject);

  void AddGameObject(PGameObject);
  
  // For Edit mode
//  void EraseGameObject(int id);
  void Clear();

  // Use this in preference to the Game when the object may not be in the local player's location
  PGameObject GetGameObject(int id); 

  // Call to change local player to new location: 
  //  objects might not be created so we wait before continuing.
  void SetLocalPlayerLocation(int newLocation);

  // Called when an object (which can't be the local player) changes its location (into or out of 
  //  the local player's location)
  // TODO params etc
  void OnObjectChangeLocation(int objId);

  void SetTimestamp(const std::string& timestamp);

  // Don't call GetFile directly - call this to queue the request.
  // That way we can retry if the request fails.
  void QGetFile(const std::string& filename);

private:
  // Local cache of objects successfully created
  bool Load();
  bool Save(); 

private:
  float m_elapsed;

  // Timestamp of last server request
  std::string m_timestamp;

  // Queue of files to download. We remove files from this queue when we know they have been
  //  successfully downloaded, and retry periodically until then.
  typedef std::set<std::string> FileQ;
  FileQ m_fileQ;

  // Asset list:
  // Objects point to asset lists.
  // Asset lists point to individual assets. We want asset lists to be updated when an asset gets downloaded.
  // When an asset list is completely downloaded we can create all the objects with that asset list.

  typedef std::set<PObject> Objects;
  Objects m_objects; // objects which we have, or are in the process of downloading/creating.

  typedef std::map<std::string, PAssetList> AssetLists;
  AssetLists m_assetLists;

  // All game objects, whatever their location
  GameObjects m_allGameObjects;

  // Active Location: only objects in this location are drawn by SceneGraph or updated by Game.
  // (GameObjects with no location ID count as being in all locations)
  int m_location;
};

typedef Singleton<ObjectManager> TheObjectManager;
}

#endif

