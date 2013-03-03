#include "StateUpdate.h"
#include <iostream>
#include <Xml/XmlNodeInterface.h>
#include <Xml/XmlParser2.h>
#include "ObjectUpdater.h"
#include <SafeUtils.h>
#include <StringUtils.h>
#include "GSNetError.h"
#include "PlayerNames.h"

namespace Amju
{
GetStateUpdatesReq::GetStateUpdatesReq(const std::string& url) : Ve1Req(url, "get state updates")
{
  m_critical = false;
}

void GetStateUpdatesReq::OnSuccess()
{
  // Child 0 is timestamp
  //PXml p = m_xml.getChildNode(0);
  //Assert(SafeStrCmp(p.getName(), "now"));
  //std::string timestamp = p.getText();
//std::cout << "Got new state update timestamp (from server): " << timestamp << "\n";

  TheObjectUpdater::Instance()->SetTimestampUpdate(m_timestamp);

  PXml p = m_xml.getChildNode(1);
  if (SafeStrCmp(p.getName(), "states"))
  {
#ifdef XML_DEBUG
std::cout << "found states element\n";
#endif

    int numObjs = p.nChildNode();

    for (int i = 0; i < numObjs; i++)
    {
      PXml obj = p.getChildNode(i);

#ifdef XML_DEBUG
std::cout << "Obj " << i << ": ";
#endif

      int id = atoi(obj.getChildNode(0).getText());
      std::string key = obj.getChildNode(1).getText();
      std::string val = obj.getChildNode(2).getText();

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
          
std::cout << "Queueing pos for object " << id << " x: " << x << " y: " << y << " z: " << z << "\n";
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
std::cout << "Got update for object " << id << " key: " << key << " val: " << val << "\n";
        TheObjectUpdater::Instance()->QueueUpdate(id, key, val);
      }

      // TODO Or in ObjectUpdater ?
      if (key == "name")
      {
        SetPlayerName(id, val);
      }
    }
  }
  else
  {
    // Unexpected response from server. Is server reachable ?
    // TODO LOG this error
    ShowError("States update: Didn't find \"states\" tag in response");
  }
}
}

