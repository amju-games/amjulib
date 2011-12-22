#include "ObjectManager.h"
#include <Timer.h>
#include "Ve1Req.h"
#include "Ve1OnlineReqManager.h"
#include <iostream>
#include <Xml/XmlNodeInterface.h>
#include <SafeUtils.h>
#include <File.h>
#include <Game.h>
#include <GameObjectFactory.h>
#include "Player.h"
#include "LocalPlayer.h"
#include <StringUtils.h>
#include "PlayerInfo.h"

//#define XML_DEBUG

namespace Amju
{
static const int MAX_CONCURRENT_DOWNLOADS = 16; // ???


std::ostream& operator<<(std::ostream& os, const Object& obj)
{
  return os << obj.m_id << " (" << obj.m_type << ")";
}

void Object::Load()
{
//  std::cout << "Loading object " << *this << "\n";

  // Create new object, load it, add to Game
  PGameObject go;

  // Check if we are creating a player - could be local or non-local
  if (m_type == Player::TYPENAME)
  {
    // If owner ID matches logged in user ID, create a LocalPlayer, else a Player.
    // TODO

    PlayerInfo* pi = ThePlayerInfoManager::Instance()->GetPI();
    if (pi->PIGetInt(PI_KEY("player obj id")) == m_id)
    {
      go = new LocalPlayer;
    }
    else
    {
      go = new Player;
    }
  }
  else
  {
    go = TheGameObjectFactory::Instance()->Create(m_type);
  }

  if (go)
  {
    go->SetId(m_id);

    // TODO Set owner

    if (m_datafile.empty())
    {
//std::cout << "Object load: no data file needed for " << *this << "\n";
      TheGame::Instance()->AddGameObject(go);
    }
    else
    {
      File f(true, File::STD); // not a glue file 
    
      if (f.OpenRead(m_datafile))
      {
//std::cout << "Object load: Opened file " << m_datafile << " ok...\n";

        if (go->Load(&f))
        {
          TheGame::Instance()->AddGameObject(go);
std::cout << "Object load: Successfully added object to game! " << *this << "\n";
        }
        else
        {
std::cout << "Object load: Failed to load game object " << *this << "\n";
        }
      }
      else
      {
std::cout << "Object load: no data file " << m_datafile << "\n";
      }
    }
  }
  else
  {
std::cout << "Object Load: Unexpected game object type: " << m_type << "\n";
  }
  m_loaded = true;
}

static std::string timestamp = "1";

class ObjectCheckReq : public Ve1Req
{
public:
  ObjectCheckReq(const std::string& url) : Ve1Req(url, "object check req")
  {
  }

  virtual void HandleResult()
  {
    // Parse XML list of objects
    // Add each new object to ObjectManager, as it now needs to download all associated assets, 
    //  then create the new object locally.

std::cout << "Got response to object check request!\n";
    HttpResult res = GetResult();

    if (!res.GetSuccess())
    {
std::cout << "OH NO FAIL! " << res.GetErrorString() << "\n";
      return; // TODO Error state
    }

    std::string str = res.GetString();
//std::cout << "Object check req result: " << str << "\n";

    // Parse XML, create Object and add to ObjectManager
    PXml xml = ParseXml(str.c_str());

    // Format of XML:
    // <time/>           <- Child(0)
    // <objs>            <- Child(1)
    //  <obj>            <- Child(1)->Child(n)
    //   <id/>           <- Child(1)->Child(n)->Child(0) etc
    //   <type/>
    //   <assetfile/>
    //   <owner/>
    //   <createtime/>
    //  </obj>
    // <objs>

    PXml p = xml.getChildNode(0);
    if (SafeStrCmp(p.getName(), "now"))
    {
      timestamp = p.getText();
    }
    else
    {
std::cout << "Object check: Didn't get time stamp in result\n";
    }

    p = xml.getChildNode(1);
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

        TheObjectManager::Instance()->AddObject(new Object(id, owner, type, assetfile, datafile));
      }
    }
    else
    {
      // Unexpected response from server. Is server reachable ?
      // TODO LOG this error
std::cout << "Didn't find \"objs\" tag..\n";
    }
  } 
};


bool AssetList::Load()
{
#ifdef ASSET_DEBUG
std::cout << "Opening asset list " << m_name << "\n";
#endif

  File f(File::NO_VERSION, File::STD); // OS file, not glue file
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
#ifdef ASSET_DEBUG
std::cout << " Got asset " << s << ", adding to ObjectManager....\n";
#endif

    const std::string s = m_assetNames[i];
    TheObjectManager::Instance()->GetFile(s);
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
  

ObjectManager::ObjectManager()
{
  m_elapsed = 999999.9f;
}

void ObjectManager::AddObject(PObject obj)
{
  if (m_objects.find(obj) != m_objects.end())
  {
    // Shouldn't happen because we are supposed to be getting new objects since the last time
    //  we checked, right ?
std::cout << "Trying to add duplicate object to object manager list!\n";
    return;
  }

  m_objects.insert(obj);

  // Get the asset list and data file if we haven't already
  GetFile(obj->m_datafile);

  // See if we have already got the asset list (because we already found another object of the same type, etc.)
  const std::string& assetlistname = obj->m_assetlist;
  if (m_assetLists.find(assetlistname) == m_assetLists.end())
  {
    // Asset List not yet created
    AssetList* assetlist = new AssetList(assetlistname);
    m_assetLists[assetlistname] = assetlist;
    GetFile(assetlistname);
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

    for (Objects::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
    {
      Object* obj = *it;
      if (obj->m_loaded)
      {
        continue;
      }
      const std::string& assetlistname = obj->m_assetlist;   
      const std::string& datafilename = obj->m_datafile;
      if (IsLocal(assetlistname) &&
          IsLocal(datafilename))
      {
        AssetList* assetlist = m_assetLists[assetlistname];
        if (assetlist->AllAssetsLoaded())
        {
          obj->Load();
        }
      }
    }

    for (AssetLists::iterator it = m_assetLists.begin(); it != m_assetLists.end(); ++it)
    {
      AssetList* assetlist = it->second;
      assetlist->Update();
    }
  }


  static const float OBJECT_CHECK_PERIOD = 60.0f; // seconds, TODO CONFIG

  // If it's time, get all the objects [in this region, TODO] created since the last check.
  // TODO Get list of all objects *deleted* since last check.

  m_elapsed += dt;
  if (m_elapsed > OBJECT_CHECK_PERIOD)
  {
    m_elapsed = 0;

    std::cout << "It's time to create a new object check req...\n";

    // Create request, add to OnlineReqManager
    std::string url = TheVe1ReqManager::Instance()->MakeUrl(GET_NEW_OBJECTS);
    url += "&time=" + timestamp;

    std::cout << "URL: " << url << "\n";

    TheVe1ReqManager::Instance()->AddReq(new ObjectCheckReq(url));
  }
}
}


