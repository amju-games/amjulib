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

namespace Amju
{
static const char* FILENAME = "object_cache.txt";

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

bool ObjectUpdater::Load()
{
std::cout << "Loading " << FILENAME << "...\n";
  File f;
  if (!f.OpenRead(FILENAME))
  {
std::cout << "...failed to open\n";

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

std::cout << "...Loaded " << num << " update items...\n";

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
    if (strs.size() != 4)
    {
      f.ReportError("Object state cache: bad position: " + s);
      continue;
    } 
    int id = ToInt(strs[0]);
    float x = ToFloat(strs[1]);
    float y = ToFloat(strs[2]);
    float z = ToFloat(strs[3]);
    QueueUpdatePos(id, Vec3f(x, y, z));
  }

std::cout << "...Loaded " << num << " positions, OK, done!\n";
  
  return true;
}

bool ObjectUpdater::Save()
{
std::cout << "Saving " << FILENAME << "...\n";

  File f;
  if (!f.OpenWrite(FILENAME))
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

std::cout << "  - Saving " << s << "\n";
  }

  // Save positions
  f.WriteComment("// positions");
  f.WriteInteger(m_posMapCache.size());
  for (PosMap::iterator it = m_posMapCache.begin(); it != m_posMapCache.end(); ++it)
  {
    const Vec3f& v = it->second;
    std::string s = ToString(it->first);
    s += ",";
    s += ToString(v.x);
    s += ",";
    s += ToString(v.y);
    s += ",";
    s += ToString(v.z);

    f.Write(s);

std::cout << "  - Saving " << s << "\n";
  }

std::cout << "...saved ok!\n";

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
  static const float POS_UPDATE_PERIOD = 10.0f; // TODO CONFIG
  if (m_posElapsed > POS_UPDATE_PERIOD)
  {
std::cout << "Creating new pos update req...\n";

    m_posElapsed = 0;

    std::string url = TheVe1ReqManager::Instance()->MakeUrl(GET_POSITION_UPDATES);
    url += "&time=" + TimestampToString(m_timestampPos); 

std::cout << "URL: " << url << "\n";

    TheVe1ReqManager::Instance()->AddReq(new PosUpdateReq(url));
  }

  m_updateElapsed += TheTimer::Instance()->GetDt();
  static const float UPDATE_PERIOD = 10.0f; // TODO CONFIG
  if (m_updateElapsed > UPDATE_PERIOD)
  {
    m_updateElapsed = 0;

    std::string url = TheVe1ReqManager::Instance()->MakeUrl(GET_STATE_UPDATES);
    url += "&time=" + TimestampToString(m_timestampUpdate); 
std::cout << "URL: " << url << "\n";
  
    TheVe1ReqManager::Instance()->AddReq(new GetStateUpdatesReq(url));
  }

  for (PosMap::iterator it = m_posMap.begin(); it != m_posMap.end();   )
  {
    int id = it->first;
    GameObject* go = TheGame::Instance()->GetGameObject(id);
    if (go)
    {
      const Vec3f& pos = it->second;

std::cout << "Object Updater: updating object " << id << " to pos: " << pos << "\n";

      Ve1Object* ve1Obj = dynamic_cast<Ve1Object*>(go);
      if (ve1Obj)
      {
        ve1Obj->MoveTo(pos);
      }
      else
      {
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

std::cout << "Object Updater: updating object " << id << " key: " << key << " val: " << val << "\n";

      ve1Obj->Set(key, val);

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

void ObjectUpdater::QueueUpdatePos(int id, const Vec3f& pos)
{
std::cout << "Queueing update pos: id: " << id << " pos: " << pos << "\n";

  m_posMap[id] = pos;
  m_posMapCache[id] = pos;
}

void ObjectUpdater::QueueUpdate(int id, const std::string& key, const std::string& val)
{
std::cout << "Queueing update: id: " << id << " key: " << key << " val: " << val << "\n";

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
  ObjUpdateReq(const std::string& url) : Ve1Req(url, "obj update req") {}

  virtual void OnSuccess()
  {
    TheObjectUpdater::Instance()->HintCheckForUpdates();   
  }
};

void ObjectUpdater::SendUpdateReq(int objId, const std::string& key, const std::string& val)
{
std::cout << "Sending state update: " << objId << " key: " << key << " val: " << val << "\n";

  static const int MAX_CONCURRENT_UPDATE_REQS = 16; // ?

  std::string url = TheVe1ReqManager::Instance()->MakeUrl(SET_STATE);
  url += "&obj_id=";
  url += ToString(objId);
  url += "&key=";
  url += key;
  url += "&val=";
  url += val; 
  TheVe1ReqManager::Instance()->AddReq(new ObjUpdateReq(url), MAX_CONCURRENT_UPDATE_REQS);
}


class MoveReq : public Ve1Req
{
public:
  MoveReq(const std::string& url, const Vec3f& pos) : Ve1Req(url, "set position"), m_requestedPos(pos)
  {
  }

  virtual void OnSuccess()
  {
std::cout << "Move req success!\n";

    // Do nothing for now, we will download the new position and cache it
    // TODO Prediction

    // It would be good to tell the Pos Updater to check now.
    TheObjectUpdater::Instance()->HintCheckForPosUpdates();   
  }

private:
  Vec3f m_requestedPos;
};


void ObjectUpdater::SendPosUpdateReq(int objId, const Vec3f& pos)
{
  std::string url = TheVe1ReqManager::Instance()->MakeUrl(SET_POSITION);
  url += "&obj_id=";
  url += ToString(objId);
  url += "&x=" + ToString(pos.x);
  url += "&y=" + ToString(pos.y);
  url += "&z=" + ToString(pos.z);

  // Only one pos request allowed at one time -- this is no good, the latest click will be discarded.
  // Need to kill any existing pos update req then add this new one. TODO
  TheVe1ReqManager::Instance()->AddReq(new MoveReq(url, pos), 1);
}
}

