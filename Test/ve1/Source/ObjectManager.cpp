#include "ObjectManager.h"
#include <Timer.h>
#include "Ve1Req.h"
#include "Ve1OnlineReqManager.h"
#include <iostream>
#include <Xml/XmlNodeInterface.h>
#include <SafeUtils.h>
#include <DownloadReq.h>
#include <Directory.h>
#include <File.h>

#define XML_DEBUG

namespace Amju
{
std::ostream& operator<<(std::ostream& os, const Object& obj)
{
  return os << obj.m_id << " (" << obj.m_type << ")";
}

void Object::Load()
{
  std::cout << "TODO Loading object " << *this << "\n";
  // TODO
}

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

    if (!GetResult().GetSuccess())
    {
std::cout << "OH NO FAIL! " << GetResult().GetErrorString() << "\n";
      return; // TODO Error state
    }

//std::cout << GetResult().GetString() << "\n";

    // Parse XML, create Object and add to ObjectManager
    PXml xml = ParseXml(GetResult().GetString().c_str());

    // Format of XML:
    // <objs>            <- Child(0)
    //  <obj>            <- Child(0)->Child(n)
    //   <id/>           <- Child(0)->Child(n)->Child(0)
    //   <type/>
    //   <assetfile/>
    //   <owner/>
    //   <createtime/>
    //  </obj>
    // <objs>

    PXml p = xml.getChildNode(0);
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
        int owner = atoi(obj.getChildNode(3).getText());

#ifdef XML_DEBUG
std::cout << " ID: " << id << ": ";
std::cout << " Type: " << type << "\n";
#endif

        TheObjectManager::Instance()->AddObject(new Object(id, owner, type, assetfile));
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

class AssetListDownloadReq : public DownloadReq
{
public:
  AssetListDownloadReq(AssetList* assetlist, const std::string& url) : 
    DownloadReq(assetlist->m_name, url, HttpClient::GET, "asset list download req"), 
    m_assetlist(assetlist) {}

  virtual void OnDownloaded()
  {
    // Now can load 
    m_assetlist->Load();
  }

private:
  AssetList* m_assetlist;
};

class AssetDownloadReq : public DownloadReq
{
public:
  AssetDownloadReq(Asset* asset, const std::string& url) : 
    DownloadReq(asset->m_name, url, HttpClient::GET, "asset download req"), 
    m_asset(asset) {}

  virtual void OnDownloaded()
  {
    // Now can load 
    m_asset->OnDownloaded();
  }

private:
  Asset* m_asset;
};

bool AssetList::Load()
{
  if (!FileExists(m_name))
  {
    // Downloaded to wrong dir ??
    std::cout << "Asset list: trying to Load, but file doesn't exist?!\n";
    return false;
  }

std::cout << "Opening asset list " << m_name << "\n";

  File f(File::STD); // OS file, not glue file
  if (!f.OpenRead(m_name))
  {
     return false;
  }
  
  m_state = AMJU_AL_LOADING;

  std::string s;
  while (f.GetDataLine(&s))
  {
    TheObjectManager::Instance()->AddAsset(s, this);    
  }

  // Now check how many assets need to download. If zero, we can load the object now.
  if (m_numAssetsDownloading == 0)
  {
    m_state = AMJU_AL_ALL_ASSETS_LOADED;
  }

  return true;
}

void AssetList::AddObject(Object* obj)
{
  switch (m_state)
  {
  case AMJU_AL_UNKNOWN:
    // File exists locally ?
    if (FileExists(m_name))
    {
std::cout << "Asset list " << m_name << " exists locally! Loading it...\n";
      bool loadok = Load();
      // Not sure what to do if we fail to load asset list. Maybe re-download it. So delete local copy.
      // TODO
      if (m_state == AMJU_AL_ALL_ASSETS_LOADED)
      {
        Assert(m_waitingObjs.empty());
std::cout << " - All assets are local too, so loading object " << *obj << "...\n";
        obj->Load();
      }
    }
    else
    {
std::cout << "Asset list " << m_name << " is not local, downloading it...\n";
      OnlineReq* assetlistdownloadreq = new AssetListDownloadReq(this, MakeDownloadUrl(this->m_name));
      TheOnlineReqManager::Instance()->AddReq(assetlistdownloadreq);
      m_state = AMJU_AL_DOWNLOADING;
    }
    break;

  case AMJU_AL_DOWNLOADING:
std::cout << "Asset list " << m_name << " is downloading...\n";
    break;

  case AMJU_AL_LOADING:
std::cout << "Asset list " << m_name << " is loaded, waiting for all assets...\n";
    break;

  case AMJU_AL_ALL_ASSETS_LOADED:
std::cout << "All assets loaded for " << m_name << " so loading object " << *obj << "\n";
    obj->Load();
    return;
  }
  
  // If we get here, we did not immediately load the object. So add object to list of waiting objs.
  m_waitingObjs.push_back(obj);
}
  
void AssetList::Inc(const std::string& assetname)
{
  m_numAssetsDownloading++;

std::cout << "AssetList " << m_name << " is waiting for " << assetname << ", count is now " << m_numAssetsDownloading << "\n";
}

void AssetList::Dec(const std::string& assetname)
{
  Assert(m_numAssetsDownloading > 0);

  m_numAssetsDownloading--;

std::cout << "AssetList " << m_name << " was waiting for " << assetname << ", has now downloaded! Count is now " << m_numAssetsDownloading << "\n";

  if (m_numAssetsDownloading == 0)
  {
std::cout << "All assets loaded for " << m_name << " so loading all waiting objects...";
    for (WaitingObjs::iterator it = m_waitingObjs.begin(); it != m_waitingObjs.end(); ++it)
    {
      Object* obj = *it;
      std::cout << " ..loading " << *obj << "\n";
      obj->Load();
    }
  }
}

void Asset::OnDownloaded()
{
  for (WaitingLists::iterator it = m_waitingLists.begin(); it != m_waitingLists.end(); ++it)
  {
    (*it)->Dec(m_name);
  }
}

void Asset::AddList(AssetList* assetlist)
{
  switch (m_state)
  {
  case AMJU_ASSET_UNKNOWN:
    if (FileExists(m_name))
    {
      m_state = AMJU_ASSET_LOCAL;
      return;
    }
    else
    {
      // Create download req
      OnlineReq* assetdownloadreq = new AssetDownloadReq(this, MakeDownloadUrl(this->m_name));
      TheOnlineReqManager::Instance()->AddReq(assetdownloadreq);
      m_state = AMJU_ASSET_DOWNLOADING;
    }
    break;

  case AMJU_ASSET_DOWNLOADING:
std::cout << "Asset " << m_name << " is downloading...\n";
    break;

  case AMJU_ASSET_LOCAL:
    // If this asset is local, do nothing. The assetlist will not have to wait for this asset.
    return;

  }

  Assert(m_state == AMJU_ASSET_DOWNLOADING);
  m_waitingLists.push_back(assetlist);
  assetlist->Inc(m_name); // increment count of assets to wait for, name is just for debugging
}

ObjectManager::ObjectManager()
{
  m_elapsed = 0;
}

void ObjectManager::AddAsset(const std::string& assetname, AssetList* assetlist)
{
  Assets::iterator it = m_assets.find(assetname);

  Asset* asset = 0;
  if (it != m_assets.end())
  {
    // This can happen as different asset lists can contain the same asset.
    asset = it->second;
  }
  else
  {
    asset = new Asset(assetname);
    m_assets[assetname] = asset;
  }

  Assert(asset);
  asset->AddList(assetlist);
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

  // See if we have already got the asset list (because we already found another object of the same type, etc.)
  const std::string& filename = obj->m_assetlist;
  AssetLists::iterator it = m_assetLists.find(filename);
  AssetList* assetlist = 0;
  if (it == m_assetLists.end())
  {
    // Asset List not yet created
    assetlist = new AssetList(filename);

    // Add to map
    m_assetLists[filename] = assetlist;

std::cout << "Creating new asset list " << filename << "\n";
  }
  else
  {
    assetlist = it->second;
    // File is either loaded or being downloaded
  }
  Assert(assetlist);

  assetlist->AddObject(obj);
}

void ObjectManager::Update()
{
  static const float OBJECT_CHECK_PERIOD = 5.0f; // seconds, TODO CONFIG

  // If it's time, get all the objects [in this region, TODO] created since the last check.
  // TODO Get list of all objects deleted since last check.

  m_elapsed += TheTimer::Instance()->GetDt();
  if (m_elapsed < OBJECT_CHECK_PERIOD)
  {
    return;
  }

std::cout << "It's time to create a new object check req...\n";

  // TODO TEMP TEST
  m_elapsed = -999990;
  // Create request, add to OnlineReqManager
  std::string url = MakeUrl(OBJECT_CHECK_REQ);
std::cout << "URL: " << url << "\n";

  TheOnlineReqManager::Instance()->AddReq(new ObjectCheckReq(url));
}
}


