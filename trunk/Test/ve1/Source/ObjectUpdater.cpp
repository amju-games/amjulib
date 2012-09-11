#include "ObjectUpdater.h"
#include <Game.h>
#include "Useful.h"
#include "Ve1Object.h"
#include <File.h>
#include <StringUtils.h>
#include "Useful.h"
#include "Ve1Req.h"
#include "Ve1OnlineReqManager.h"
#include "PosUpdate.h"
#include "StateUpdate.h"
#include <Timer.h>
#include "GSNetError.h"
#include "ObjectManager.h"
#include "LocalPlayer.h"
#include "GameMode.h"

//#define OU_DEBUG
//#define YES_USE_CACHE
//#define POS_DEBUG

namespace Amju
{
static const char* CACHE_FILENAME = "object_state_cache.txt";

ObjectUpdater::ObjectUpdater() : m_timestampPos(1), m_timestampUpdate(1)
{
  // Will fail first time we run, so timestamps will be 1, and we download *everything* from server
  Load();

  m_posElapsed = 0;
  m_updateElapsed = 0;
}

ObjectUpdater::~ObjectUpdater()
{
  Save();
}

void ObjectUpdater::Clear()
{
  m_posMap.clear();
  m_keyvalMap.clear();
  // No need to trash the cache containers ???
  m_timestampPos = 1;
  m_timestampUpdate = 1;
  m_posElapsed = 0;
  m_updateElapsed = 0;
}

bool ObjectUpdater::Load()
{
#ifdef YES_USE_CACHE

#ifdef OU_DEBUG
std::cout << "Loading " << CACHE_FILENAME << "...\n";
#endif

  File f;
  if (!f.OpenRead(CACHE_FILENAME))
  {
    return false;
  }
  if (!m_timestampPos.Load(&f))
  {
    f.ReportError("Object state cache: Expected pos timestamp");
    return false;
  }

  if (!m_timestampUpdate.Load(&f))
  {
    f.ReportError("Object state cache: Expected update timestamp");
    return false;
  }

  int num = 0;

  // Load key/val pairs
  if (!f.GetInteger(&num))
  {
    f.ReportError("Object state cache: Expected num items");
    return false;
  }
  for (int i = 0; i < num; i++)
  {
    std::string s;
    if (!f.GetDataLine(&s))
    {
      f.ReportError("Object state cache: Expected item");
      return false;
    }
    // Format is id, key, val
    Strings strs = Split(s, ',');
    if (strs.size() != 3)
    {
      // Delimiter in key or val ??
      f.ReportError("Object state cache: bad item line: " + s);
      continue; // try to keep going
    }   
    int id = ToInt(strs[0]);
    QueueUpdate(id, strs[1], strs[2]);
  }

#ifdef OU_DEBUG
std::cout << "...Loaded " << num << " update items...\n";
#endif

  // Load positions
  if (!f.GetInteger(&num))
  {
    f.ReportError("Object state cache: Expected num positions");
    return false;
  }
  for (int i = 0; i < num; i++)
  {
    std::string s;
    if (!f.GetDataLine(&s))
    {
      f.ReportError("Object state cache: Expected position");
      return false;
    }
    // Format is id,x, y, z
    Strings strs = Split(s, ',');
    if (strs.size() != 5)
    {
      f.ReportError("Object state cache: bad position: " + s);
      continue;
    } 
    int id = ToInt(strs[0]);
    float x = ToFloat(strs[1]);
    float y = ToFloat(strs[2]);
    float z = ToFloat(strs[3]);
    int location = ToInt(strs[4]);

    QueueUpdatePos(id, Vec3f(x, y, z), location);
  }

#ifdef OU_DEBUG
std::cout << "...Loaded " << num << " positions, OK, done!\n";
#endif
  
#endif // YES_USE_CACHE

  return true;
}

bool ObjectUpdater::Save()
{
#ifdef YES_USE_CACHE

std::cout << "Saving " << CACHE_FILENAME << "...\n";

  File f;
  if (!f.OpenWrite(CACHE_FILENAME))
  {
    return false;
  }

  f.WriteComment("// timestamp");
  if (!m_timestampPos.Save(&f))
  {
    f.ReportError("Object state cache: failed to save pos timestamp");
    return false;
  }

  if (!m_timestampUpdate.Save(&f))
  {
    f.ReportError("Object state cache: failed to save update timestamp");
    return false;
  }

  // Save key/val pairs
  f.WriteComment("// key/val pairs");
  f.WriteInteger(m_keyvalMapCache.size());
  for (KeyValMap::iterator it = m_keyvalMapCache.begin(); it != m_keyvalMapCache.end(); ++it)
  {
    std::string s = ToString(it->first.first);
    s += ",";
    s += it->first.second;
    s += ",";
    s += it->second;

    f.Write(s);

#ifdef OU_DEBUG
std::cout << "  - Saving " << s << "\n";
#endif
  }

  // Save positions
  f.WriteComment("// positions");
  f.WriteInteger(m_posMapCache.size());
  for (PosMap::iterator it = m_posMapCache.begin(); it != m_posMapCache.end(); ++it)
  {
    const Vec3f& v = it->second.pos;
    int location = it->second.location;

    std::string s = ToString(it->first);
    s += ",";
    s += ToString(v.x);
    s += ",";
    s += ToString(v.y);
    s += ",";
    s += ToString(v.z);
    s += ",";
    s += ToString(location);

    f.Write(s);

#ifdef OU_DEBUG
std::cout << "  - Saving " << s << "\n";
#endif
  }

#ifdef OU_DEBUG
std::cout << "...saved ok!\n";
#endif

#endif // YES_USE_CACHE

  return true;
}

void ObjectUpdater::HintCheckForUpdates()
{
  m_posElapsed = 9999.9f;
}

void ObjectUpdater::HintCheckForPosUpdates()
{
  m_updateElapsed = 9999.9f;
}

void ObjectUpdater::Update()
{
  // Get pos updates periodically.
  // TODO Only get objects whose position has changed since last check ?
  m_posElapsed += TheTimer::Instance()->GetDt();

  // TODO These poll periods should depend on network latency/bandwidth
  static const float POS_UPDATE_PERIOD = 1.0f; // TODO CONFIG
  if (m_posElapsed > POS_UPDATE_PERIOD) //// && IsOnline())
  {
//std::cout << "Creating new pos update req...\n";

    m_posElapsed = 0;

    std::string url = TheVe1ReqManager::Instance()->MakeUrl(GET_POSITION_UPDATES);
    url += "&time=" + TimestampToString(m_timestampPos); 

//std::cout << "URL: " << url << "\n";

    TheVe1ReqManager::Instance()->AddReq(new PosUpdateReq(url));
  }

  m_updateElapsed += TheTimer::Instance()->GetDt();
  static const float UPDATE_PERIOD = 1.0f; // TODO CONFIG
  if (m_updateElapsed > UPDATE_PERIOD) //// && IsOnline())
  {
    m_updateElapsed = 0;

    std::string url = TheVe1ReqManager::Instance()->MakeUrl(GET_STATE_UPDATES);
    url += "&time=" + TimestampToString(m_timestampUpdate); 
  
    TheVe1ReqManager::Instance()->AddReq(new GetStateUpdatesReq(url));
  }

  for (PosMap::iterator it = m_posMap.begin(); it != m_posMap.end();   )
  {
    int id = it->first;
    // The object may exist in ObjectManager but not in Game, as it may not be in local player
    // location!
    GameObject* go = TheObjectManager::Instance()->GetGameObject(id);
    if (go)
    {
      const Vec3f& pos = it->second.pos;
      int location = it->second.location;

#ifdef POS_DEBUG
std::cout << "Object Updater: updating object " << id << " to pos: " << pos << " loc: " << location << "\n";
#endif

      Ve1Object* ve1Obj = dynamic_cast<Ve1Object*>(go);
      if (ve1Obj)
      {
        int vloc = ve1Obj->GetLocation();

        // If this msg is for local player and we are in a different location, discard this msg.

        if (ve1Obj->GetId() == GetLocalPlayerId() && vloc != -1)
        {
          // Ignore all pos updates for local player ???!?
        }
        else
        if (location == vloc)
        {
#ifdef POS_DEBUG
std::cout << "Moving object " << ve1Obj->GetId() << ", within same location " << location << "\n";
#endif
          ve1Obj->MoveTo(pos);
        }
        else
        {
#ifdef POS_DEBUG
std::cout << "Moving object " << ve1Obj->GetId() << " to NEW location " << location << "\n";
#endif

          // TODO We want other players who leave the current location to go to the last place in the
          //  current location before disappearing - so they will appear to go through a portal, not
          //  just vanish. So don't set this pos/location until they have reached their previous 
          //  destination in the current location - just continue, so we will check again.
          // TODO
          // E.g. if (ve1Obj->IsMoving()) { ++it; continue; }
          // TODO But watch out, make sure there is a safety net so all objects are correctly located
          //  eventually!
          ve1Obj->SetPos(pos);
          ve1Obj->SetLocation(location);
        }
      }
      else
      {
        // TODO what to do, can't set location
        Assert(0);
std::cout << "..using SetPos, not a Ve1Object\n";
        go->SetPos(pos);
      }

#ifdef WIN32
      it = m_posMap.erase(it);
#else
      m_posMap.erase(it);
      ++it;
#endif
    }
    else
    {
//std::cout << "No GameObject with ID " << id << "\n";
      ++it;
    }
  }

  for (KeyValMap::iterator it = m_keyvalMap.begin(); it != m_keyvalMap.end();   )
  {
    int id = it->first.first;

    GameObject* go = TheGame::Instance()->GetGameObject(id);
    Ve1Object* ve1Obj = dynamic_cast<Ve1Object*>(go);
    Assert(!go || (go && ve1Obj));
  
    if (ve1Obj)
    {
      const std::string& key = it->first.second;
      const std::string& val = it->second; 

//std::cout << "Object Updater: updating object " << id << " key: " << key << " val: " << val << "\n";

      ve1Obj->SetKeyVal(key, val);

#ifdef WIN32
      it = m_keyvalMap.erase(it);
#else
      m_keyvalMap.erase(it);
      ++it;
#endif
      
    }
    else
    {
      ++it;
    }
        
  }
}

void ObjectUpdater::QueueUpdatePos(int id, const Vec3f& pos, int location)
{
#ifdef QUEUE_DEBUG
std::cout << "Queueing update pos: id: " << id << " pos: " << pos << " location: " << location << "\n";
#endif

  m_posMap[id] = PosLocation(pos, location);
  m_posMapCache[id] = PosLocation(pos, location);
}

void ObjectUpdater::QueueUpdate(int id, const std::string& key, const std::string& val)
{
#ifdef QUEUE_DEBUG
std::cout << "Queueing update: id: " << id << " key: " << key << " val: " << val << "\n";
#endif

  IdKey idkey(id, key);
  m_keyvalMap[idkey] = val;
  m_keyvalMapCache[idkey] = val;
}

void ObjectUpdater::SetTimestampPos(Timestamp timestamp)
{
  m_timestampPos = timestamp;
}

void ObjectUpdater::SetTimestampUpdate(Timestamp timestamp)
{
  m_timestampUpdate = timestamp;
}


class ObjUpdateReq : public Ve1Req
{
public:
  ObjUpdateReq(const std::string& url, const std::string& name) : Ve1Req(url, name.c_str()) {}

  virtual void OnSuccess()
  {
    // We have successfully updated the server. 
    TheObjectUpdater::Instance()->HintCheckForUpdates();   

    CheckCollects();
  }
};

void ObjectUpdater::SendUpdateReq(int objId, const std::string& key, const std::string& val)
{
//std::cout << "Sending state update: " << objId << " key: " << key << " val: " << val << "\n";

  static const int MAX_CONCURRENT_UPDATE_REQS = 1000; // Would be a problem if we didn't send..??

  std::string url = TheVe1ReqManager::Instance()->MakeUrl(SET_STATE);
  url += "&obj_id=";
  url += ToString(objId);
  url += "&key=";
  url += key;
  url += "&val=";
  url += val; 

  std::string reqName = "obj_up_" + ToString(objId) + "_" + key;

  // Discard all but most recent
  TheVe1ReqManager::Instance()->AddReq(new ObjUpdateReq(url, reqName), 1, false);
}


class MoveReq : public Ve1Req
{
public:
  MoveReq(const std::string& url, const std::string& name, const Vec3f& pos) : 
    Ve1Req(url, name.c_str()), m_requestedPos(pos)
  {
  }

  virtual void OnSuccess()
  {
    // Do nothing for now, we will download the new position and cache it
    // TODO Prediction

    // It would be good to tell the Pos Updater to check now.
    TheObjectUpdater::Instance()->HintCheckForPosUpdates();   
  }

private:
  Vec3f m_requestedPos;
};

static const int MAX_POS_UPDATE_REQS = 2; // for mad clicking 

void ObjectUpdater::SendPosUpdateReq(int objId, const Vec3f& pos, int location)
{
  // TODO just to track down odd coords
  Assert(fabs(pos.x) < 2000.0f);

  std::string url = TheVe1ReqManager::Instance()->MakeUrl(SET_POSITION);
  url += "&obj_id=";
  url += ToString(objId);
  url += "&x=" + ToString(pos.x);
  url += "&y=" + ToString(pos.y);
  url += "&z=" + ToString(pos.z);
  url += "&loc=" + ToString(location);

  // Only one pos request allowed at one time -- this is no good, the latest click will be discarded.
  // Need to kill any existing pos update req then add this new one. 
  // Allow 2 in case one is in flight. Then allow only one more. Delete any older waiting positions
  //  FOR THIS OBJECT.
  std::string reqName = "setpos_" + ToString(objId); 
  TheVe1ReqManager::Instance()->AddReq(new MoveReq(url, reqName, pos), MAX_POS_UPDATE_REQS, false);
}

void ObjectUpdater::SendChangeLocationReq(int objId, const Vec3f& pos, int location)
{
  std::string url = TheVe1ReqManager::Instance()->MakeUrl(CHANGE_LOCATION);
  url += "&obj_id=";
  url += ToString(objId);
  url += "&x=" + ToString(pos.x);
  url += "&y=" + ToString(pos.y);
  url += "&z=" + ToString(pos.z);
  url += "&loc=" + ToString(location);

  std::string reqName = "setpos_" + ToString(objId); 
  TheVe1ReqManager::Instance()->AddReq(new MoveReq(url, reqName, pos), MAX_POS_UPDATE_REQS, false);
}
}

