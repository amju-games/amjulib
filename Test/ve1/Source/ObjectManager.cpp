#include "ObjectManager.h"
#include <Timer.h>
#include "Ve1Req.h"
#include "Ve1OnlineReqManager.h"
#include <iostream>
#include <SafeUtils.h>
#include <File.h>
#include <Game.h>
#include <GameObjectFactory.h>
#include <Directory.h>
#include "Player.h"
#include "LocalPlayer.h"
#include <StringUtils.h>
#include "PlayerInfo.h"
#include "GSNetError.h"
#include <SAP.h>
#include "Ve1SceneGraph.h"
#include "GSFileUpdateCheck.h"
#include "GSWaitForNewLocation.h"
#include "Terrain.h"
#include "ObjectUpdater.h"
#include "GameMode.h"
#include "Useful.h"

//#define XML_DEBUG
#define ASSET_DEBUG
//#define OBJECT_CHECK_DEBUG

namespace Amju
{
std::ostream& operator<<(std::ostream& os, const Object& obj)
{
  return os << obj.m_id << " (" << obj.m_type << ")";
}

void AddGameObjectToGame(GameObject* go)
{
  static Game* g = TheGame::Instance();

  Assert(go);

  // Remove existing, with warning
  int id = go->GetId();
  if (g->GetGameObject(id))
  {
std::cout << "***WARNING: GameObject " << *go << " already in Game container, erasing and re-adding!\n";
      TheGame::Instance()->EraseGameObject(id);
  }

  TheGame::Instance()->AddGameObject(go);
}

bool Object::Save(File* f)
{
  return (
    f->WriteComment("// Object id, owner, type, assetlist, datafile") &&
    f->WriteInteger(m_id) &&
    f->WriteInteger(m_owner) &&
    f->Write(m_type) &&
    f->Write(m_assetlist) &&
    f->Write(m_datafile) );
}

bool Object::Load(File* f)
{
  if (!f->GetInteger(&m_id)) 
  {
    f->ReportError("Object create cache: Expected object id");
    return false;
  } 
  if (!f->GetInteger(&m_owner))
  {
    f->ReportError("Object create cache: Expected object owner");
    return false;
  }
  if (!f->GetDataLine(&m_type))
  {
    f->ReportError("Object create cache: Expected object type");
    return false;
  }
  if (!f->GetDataLine(&m_assetlist))
  {
    f->ReportError("Object create cache: Expected object assetlist");
    return false;
  }
  if (!f->GetDataLine(&m_datafile))
  {
    f->ReportError("Object create cache: Expected object datafile");
    return false;
  }
  return true;
}

void Object::Create()
{
//  std::cout << "Loading object " << *this << "\n";

  // Create new object, load it, add to Game
  PGameObject go = TheGameObjectFactory::Instance()->Create(m_type);

  if (go)
  {
    go->SetId(m_id);

    // TODO Set owner

    if (m_datafile.empty() || m_datafile == "none")
    {
//std::cout << "Object load: no data file needed for " << *this << "\n";
      TheObjectManager::Instance()->AddGameObject(go);
    }
    else
    {
      File f(true, File::STD); // not a glue file 
    
      if (f.OpenRead(m_datafile))
      {
//std::cout << "Object load: Opened file " << m_datafile << " ok...\n";

        if (go->Load(&f))
        {
//std::cout << "Object load: Successfully loaded game object " << *this << "... ";
          TheObjectManager::Instance()->AddGameObject(go);
        }
        else
        {
std::cout << "Object load: Failed to load game object " << *this << "\n";
        }
      }
      else
      {
//std::cout << "Object load: no data file " << m_datafile << "\n";
      }
    }
  }
  else
  {
std::cout << "Object Load: Unexpected game object type: " << m_type << "\n";
  }
  m_loaded = true;
}


class ObjectCheckReq : public Ve1Req
{
public:
  ObjectCheckReq(const std::string& url) : Ve1Req(url, "object check req")
  {
  }

  virtual void OnSuccess()
  {
    // Add each new object to ObjectManager, as it now needs to download all associated assets, 
    //  then create the new object locally.

#ifdef OBJECT_CHECK_DEBUG
std::cout << "Got successful response to object check request!\n";
#endif

    // Format of XML:
    // <now/>           <- Child(0)
    // <objs>            <- Child(1)
    //  <obj>            <- Child(1)->Child(n)
    //   <id/>           <- Child(1)->Child(n)->Child(0) etc
    //   <type/>
    //   <assetfile/>
    //   <owner/>
    //   <createtime/>
    //  </obj>
    // <objs>

    PXml p = m_xml.getChildNode(0);
    Assert(SafeStrCmp(p.getName(), "now"));
    std::string timestamp = p.getText();

    TheObjectManager::Instance()->SetTimestamp(timestamp);

    p = m_xml.getChildNode(1);
    if (SafeStrCmp(p.getName(), "objs"))
    {
#ifdef XML_DEBUG
std::cout << "found objs element\n";
#endif

      int numObjs = p.nChildNode();

      for (int i = 0; i < numObjs; i++)
      {
        PXml obj = p.getChildNode(i);

#ifdef XML_DEBUG
std::cout << "Obj " << i << ": ";
#endif

        int id = atoi(obj.getChildNode(0).getText());
        std::string type = obj.getChildNode(1).getText();
        std::string assetfile = obj.getChildNode(2).getText();
        std::string datafile = obj.getChildNode(3).getText();
        int owner = atoi(obj.getChildNode(4).getText());

#ifdef XML_DEBUG
std::cout << " ID: " << id << ": ";
std::cout << " Type: " << type << "\n";
#endif

//std::cout << "NEW OBJECT! ID: " << id << " Type: " << type << " Asset file: " << assetfile << " Data file: " << datafile << " Owner: " << owner << "\n";

        TheObjectManager::Instance()->AddObject(new Object(id, owner, type, assetfile, datafile));
      }
    }
    else
    {
      // Unexpected response from server. Is server reachable ?
      // TODO LOG this error
      ShowError("Didn't find \"objs\" tag in response");
    }
  } 
};


bool AssetList::Load()
{
  if (m_name.empty() || m_name == "none")
  {
    m_state = AMJU_AL_ALL_ASSETS_LOADED;
    return true;
  }

#ifdef ASSET_DEBUG
std::cout << "Opening asset list " << m_name << "\n";
#endif

  File f(true, File::STD); // OS file, not glue file
  if (!f.OpenRead(m_name))
  {
std::cout << "Failed to open asset list file " << m_name << "\n";
     return false;
  }
  
  m_state = AMJU_AL_LOADING;

  std::string s;
  while (f.GetDataLine(&s))
  {
    m_assetNames.push_back(s);
  }
  int size = m_assetNames.size();

  for (int i = 0; i < size; i++)
  {
    const std::string s = m_assetNames[i];

#ifdef ASSET_DEBUG
std::cout << " Got asset " << s << ", adding to ObjectManager....\n";
#endif

    TheObjectManager::Instance()->QGetFile(s);
  }

  return true;
}

void AssetList::Update()
{
  switch (m_state)
  {
  case AMJU_AL_ALL_ASSETS_LOADED:
    return;

  case AMJU_AL_UNKNOWN:
    if (TheObjectManager::Instance()->IsLocal(m_name))
    {
      m_state = AMJU_AL_LOADING;
      Load();
    }
    break;

  case AMJU_AL_LOADING:
    for (AssetNames::iterator it = m_assetNames.begin(); it != m_assetNames.end(); ++it)
    {
      if (!TheObjectManager::Instance()->IsLocal(*it))
      {
        return;
      }
    }
    m_state = AMJU_AL_ALL_ASSETS_LOADED;
  }
}
  
static const char* FILENAME = "objects_created_cache.txt";

ObjectManager::ObjectManager()
{
  m_timestamp = "1";

  m_elapsed = 999999.9f;
  m_location = -1;

  if (!Load())
  {
std::cout << "ObjectManager cache load failed\n";

    //if (!IsOnline())
    //{
    //std::cout << "CAN'T CONTINUE, NO CACHE AND NOT ONLINE!!\n";
    //  // TODO Go to error state ?
    //}

    m_objects.clear();
    m_timestamp = "1";
  }
}

ObjectManager::~ObjectManager()
{
  Save();
}

void ObjectManager::SetTimestamp(const std::string& t)
{
  m_timestamp = t;
}

bool ObjectManager::Load()
{
  File f;
  if (!f.OpenRead(FILENAME))
  {
    return false;
  }
  if (!f.GetDataLine(&m_timestamp))
  {
    f.ReportError("Object create cache: expected timestamp");
    return false;
  }
  int numObjs = 0;
  if (!f.GetInteger(&numObjs))
  {
    f.ReportError("Object create cache: Expected num objects");
    return false;
  }
//std::cout << "Object create cache: got " << numObjs << " objs!\n";

  for (int i = 0; i < numObjs; i++)
  {
    PObject obj = new Object;
    if (!obj->Load(&f))
    {
      return false;
    }
//std::cout << " - Adding object " << *obj << "\n";

    AddObject(obj);
  }
//std::cout << "Loaded object create cache ok!\n";
  return true;
}

bool ObjectManager::Save()
{
std::cout << "Saving object create cache... ";

  // Write contents of object cache to file
  File f;
  if (!f.OpenWrite(FILENAME))
  {
    return false;
  }

  f.WriteComment("// Timestamp");
  f.Write(m_timestamp);

  int num = m_objects.size(); // no need for separate Cache container, right ?
  f.WriteInteger(num);
  // Iterate over objects, save members for each 
  for (Objects::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
  {
    if (!(*it)->Save(&f))
    {
std::cout << "FAILED to save object create cache!!\n";
      return false;
    }
  }
std::cout << "Saved object create cache ok!\n";
  return true;
}

void ObjectManager::QGetFile(const std::string& filename)
{
  m_fileQ.insert(filename);
}

void ObjectManager::AddObject(PObject obj)
{
  if (m_objects.find(obj) != m_objects.end())
  {
    // Shouldn't happen because we are supposed to be getting new objects since the last time
    //  we checked, right ?
    if (GetGameMode() == AMJU_MODE_EDIT)
    {
std::cout << "*** INFO: Overwriting object " << obj->m_id << " in ObjectManager\n";
    }
    else
    {
std::cout << "Trying to add duplicate object to object manager list!\n";
      return;
    }
  }

  // Add this object to cache -- contains all objects ever created
  // TODO Is this necessary ?
////  m_objectCache.insert(obj);

  m_objects.insert(obj);

  // Get the asset list and data file if we haven't already
  if (!obj->m_datafile.empty() && obj->m_datafile != "none")
  {
    QGetFile(obj->m_datafile);
  }

  // See if we have already got the asset list (because we already found another object of the same type, etc.)
  const std::string& assetlistname = obj->m_assetlist;
  if (!assetlistname.empty() && assetlistname != "none")
  {
    if (m_assetLists.find(assetlistname) == m_assetLists.end())
    {
      // Asset List not yet created
      AssetList* assetlist = new AssetList(assetlistname);
      m_assetLists[assetlistname] = assetlist;
      QGetFile(assetlistname);
    }
  }

  // Later we check state of asset list and datafile...
}

void ObjectManager::Update()
{
  float dt = TheTimer::Instance()->GetDt();

  // Check state of files 
  // should be periodic, not every frame
  // TODO Only need to do this if there are oustanding requests ?

  static const float FILE_CHECK_TIME = 1.0f; // secs  TODO CONFIG
  static float fileCheckTime = 0;
  fileCheckTime += dt;
  if (fileCheckTime > FILE_CHECK_TIME)
  {
    fileCheckTime = 0;

    // Check the file download queue
    int i = 0;
    for (FileQ::iterator it = m_fileQ.begin(); it != m_fileQ.end(); )
    {
      if (i > MAX_CONCURRENT_DOWNLOADS)
      {
        break;
      }
      const std::string& filename = *it;
      if (IsLocal(filename))
      {
std::cout << "FILE Q: already local: " << filename << "\n";

        // Remove this filename, we have successfully downloaded it 
#ifdef WIN32
        it = m_fileQ.erase(it);
#else
        m_fileQ.erase(it);
        ++it;
#endif
      }
      else
      {
std::cout << "FILE Q: trying to download file: " << *it << "\n";

        GetFile(*it); // really call GetFile()
        ++it;
		if (!FileExists(File::GetRoot() + filename))
		{
          i++; // inc count of requests
		}
      }
    }


    for (Objects::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
    {
      Object* obj = *it;
      if (obj->m_loaded)
      {
        continue;
      }

      const std::string& assetlistname = obj->m_assetlist;   
      const std::string& datafilename = obj->m_datafile;

      if ((assetlistname.empty() || assetlistname == "none") &&
          (datafilename.empty() || datafilename == "none"))
      {
        obj->Create();
        TheObjectUpdater::Instance()->HintCheckForUpdates();
        TheObjectUpdater::Instance()->HintCheckForPosUpdates();
      }
      else if (IsLocal(assetlistname) && IsLocal(datafilename))
      {
        AssetList* assetlist = m_assetLists[assetlistname];
        Assert(assetlist);
        if (assetlist->AllAssetsLoaded())
        {
          obj->Create();
          // Hint to get pos/state for this object
          // Doesn't seem to help speed up Waiting screen
          TheObjectUpdater::Instance()->HintCheckForUpdates();
          TheObjectUpdater::Instance()->HintCheckForPosUpdates();
        }
      }
    }

    for (AssetLists::iterator it = m_assetLists.begin(); it != m_assetLists.end(); ++it)
    {
      AssetList* assetlist = it->second;
      assetlist->Update();
    }
  }

  static const float OBJECT_CHECK_PERIOD = 3.0f; // seconds, TODO CONFIG

  // If it's time, get all the objects [in this region, TODO] created since the last check.
  // TODO Get list of all objects *deleted* since last check.

  m_elapsed += dt;
  if (m_elapsed > OBJECT_CHECK_PERIOD)
  {
    m_elapsed = 0;

//std::cout << "It's time to create a new object check req...\n";

    // Create request, add to OnlineReqManager
    std::string url = TheVe1ReqManager::Instance()->MakeUrl(GET_NEW_OBJECTS);
    url += "&time=" + m_timestamp;

//std::cout << "URL: " << url << "\n";

    TheVe1ReqManager::Instance()->AddReq(new ObjectCheckReq(url));
  }
}

void ObjectManager::AddGameObject(PGameObject go)
{
  // Already added ?
  Assert(m_allGameObjects.find(go->GetId()) == m_allGameObjects.end());

  m_allGameObjects[go->GetId()] = go;
//std::cout << "Created game object " << go->GetId() << " " << go->GetTypeName() << " but don't know its location yet\n"; 
}

void ObjectManager::OnObjectChangeLocation(int objId)
{
  // No such object - OK if we are in edit mode and have just created it
  if (m_allGameObjects.find(objId) == m_allGameObjects.end())
  {
    return;
  }

  // TODO Handle objects leaving the local player location

  Assert(m_allGameObjects.find(objId) != m_allGameObjects.end());
  PGameObject go = m_allGameObjects[objId];

  Ve1Object* v = dynamic_cast<Ve1Object*>(go.GetPtr());
  if (v)
  {
    if (v->GetLocation() == m_location)
    {
      AddGameObjectToGame(go); ////TheGame::Instance()->AddGameObject(go);
std::cout << "Added game object " << go->GetId() << " (" << go->GetTypeName() << ") to our location (" << m_location << ")\n"; 

      if (GetGameMode() == AMJU_MODE_EDIT)
      {
        v->CreateEditNode();
      }

      v->OnLocationEntry();
    }
    else
    {
std::cout << "Game object " << objId << " changed location but is not in our location (" << m_location << ")\n"; 
      TheGame::Instance()->EraseGameObject(objId);
      v->OnLocationExit();
    }
  }
  else
  {
    AddGameObjectToGame(go); ////TheGame::Instance()->AddGameObject(go);
  }
}

void ObjectManager::SetLocalPlayerLocation(int newLocation)
{
std::cout << "#¢#¢#¢ In ObjectManager::SetLocalPlayerLocation...\n";

  if (m_location == newLocation)
  {
std::cout << "Er, setting location to current value!\n";
    // TODO Commenting this out doesn't fix problem :-(
    if (GetGameMode() != AMJU_MODE_EDIT)
    {
      return;
    }
  }

  // Why not do this ??
  if (GetLocalPlayer())
  {
    GetLocalPlayer()->SetLocation(newLocation);
  }

  m_location = newLocation;

  TheGame::Instance()->ClearGameObjects();
  // If using Sweep and Prune for collisions, clear list of objects
  ////TheSAP::Instance()->Clear();

  ClearVe1SceneGraph();

  // Clear the current terrain, which is the important thing the new location must have!
  ClearTerrain();

//std::cout << " ..old location trashed, adding game objects in new location...\n";

  for (GameObjects::iterator it = m_allGameObjects.begin(); it != m_allGameObjects.end(); ++it)
  {
    PGameObject go = it->second;
    Ve1Object* v = dynamic_cast<Ve1Object*>(go.GetPtr());
    if (v)
    {
      if (v->GetLocation() == m_location)
      {
//std::cout << " ..Game object: " << go->GetTypeName() << ", id: " << go->GetId() << "\n";
        AddGameObjectToGame(go); ////TheGame::Instance()->AddGameObject(go);
        ////TheSAP::Instance()->AddBox(go);
        v->OnLocationEntry();
      }
    }
    else
    {
std::cout << "Rather unexpected type of game object: " << go->GetTypeName() << ", id: " << go->GetId() << "\n";
      AddGameObjectToGame(go); ////TheGame::Instance()->AddGameObject(go);
    } 
  }

  // Change to waiting state: some objects required for this new location might not exist yet
  TheGame::Instance()->SetCurrentState(TheGSWaitForNewLocation::Instance());
}

PGameObject ObjectManager::GetGameObject(int id)
{
  GameObjects::iterator it = m_allGameObjects.find(id);
  if (it == m_allGameObjects.end())
  {
    return 0; // assume this is allowed, to check if an object exists or not
  }
  return it->second;
}
}


