#include <iostream>
#include <AmjuFirst.h>
#include <XmlNodeInterface.h>
#include <XmlParser2.h>
#include <SafeUtils.h>
#include <StringUtils.h>
#include "StateUpdate.h"
#include "ObjectUpdater.h"
#include "GSNetError.h"
#include "PlayerNames.h"
#include <AmjuFinal.h>

//#define POS_UPDATE_DEBUG
//#define UPDATE_DEBUG
//#define XML_DEBUG
//#define UPDATE_DEBUG_ALREADY_QUEUED

namespace Amju
{
static Timestamp lastFastTimestamp = 1;

void QueueUpdates(PXml p)
{
    int numObjs = p.nChildNode();

    Timestamp maxTimestamp = lastFastTimestamp;
//std::cout << "LastFastTimeStamp: " << lastFastTimestamp.ToSeconds() << "\n";

    for (int i = 0; i < numObjs; i++)
    {
      PXml obj = p.getChildNode(i);

#ifdef XML_DEBUG
std::cout << "Obj " << i << ": ";
#endif
      if (obj.nChildNode() < 3)
      {
std::cout << "Expected more children in node.\n";
        continue;
      }
      int id = atoi(obj.getChildNode(0).getText());
      std::string key = obj.getChildNode(1).getText();
      std::string val = obj.getChildNode(2).getText();
      if (obj.nChildNode() > 3)
      {
        // Timestamp - check if we have already seen this update
        Timestamp t = ToInt(obj.getChildNode(3).getText()); 
        if (t <= lastFastTimestamp) 
        {
#ifdef UPDATE_DEBUG_ALREADY_QUEUED
std::cout << "This req has already been queued: id: " 
  << id << " key: " << key << " val: " << val << "\n";
#endif
          continue;
        }
        if (maxTimestamp < t)
        {
          maxTimestamp = t;
        }
      }

      // TODO Sanity check ?

      if (key == "pos")
      {
        Strings strs = Split(val, ',');
        if (strs.size() == 4)
        {
          float x = ToFloat(strs[0]);
          float y = ToFloat(strs[1]);
          float z = ToFloat(strs[2]);
          int loc = ToInt(strs[3]);

#ifdef POS_UPDATE_DEBUG          
std::cout << "Queueing pos for object " << id << " x: " << x << " y: " << y << " z: " << z << "\n";
#endif

          TheObjectUpdater::Instance()->QueueUpdatePos(id, Vec3f(x, y, z), loc);
        }
        else
        {
std::cout << "Unexpected bad pos: " << val << " for pos for ID " << id << "\n";
          Assert(0);
        }
      }
      else
      {
#ifdef UPDATE_DEBUG          
std::cout << "Got update for object " << id << " key: " << key << " val: " << val << "\n";
#endif
        TheObjectUpdater::Instance()->QueueUpdate(id, key, val);
      }

      // TODO Or in ObjectUpdater ?
      if (key == "name")
      {
        SetPlayerName(id, val);
      }
    }
    lastFastTimestamp = maxTimestamp;
}

GetStateUpdatesReq::GetStateUpdatesReq(const std::string& url) : Ve1Req(url, "get state updates")
{
  m_critical = false;
}

GetFastStateUpdatesReq::GetFastStateUpdatesReq(const std::string& url) :
  Ve1Req(url, "fast update")
{
  m_critical = false;
}

void GetFastStateUpdatesReq::HandleResult()
{
#ifdef FAST_UPDATE_DEBUG
  std::cout << "Got response for fast update: "  << GetResult().GetString() << "\n";
#endif

  const HttpResult& res = GetResult();
  if (res.GetSuccess())
  {
    const std::string& str = res.GetString();
    m_xml = ParseXml(str.c_str());

    PXml p = m_xml.getChildNode(0);
   
    if (p.nChildNode())
    {
      QueueUpdates(p); 
    }
    else
    {
#ifdef FAST_UPDATE_DEBUG
std::cout << "Failed to parse response to fast update.\n";
#endif
    }
  }
}

void GetStateUpdatesReq::OnSuccess()
{
#ifdef UPDATE_DEBUG          
  {
  // Child 0 is timestamp
  PXml p = m_xml.getChildNode(0);
  Assert(SafeStrCmp(p.getName(), "now"));
  std::string timestamp = p.getText();
std::cout << "Got new state update timestamp (from server): " << timestamp << "\n";
  }
#endif

  TheObjectUpdater::Instance()->SetTimestampUpdate(m_timestamp);

  PXml p = m_xml.getChildNode(1);
  if (SafeStrCmp(p.getName(), "states"))
  {
#ifdef XML_DEBUG
std::cout << "found states element\n";
#endif

    QueueUpdates(p);
  }
  else
  {
    // Unexpected response from server. Is server reachable ?
    // TODO LOG this error
    ShowError("States update: Didn't find \"states\" tag in response");
  }
}
}


