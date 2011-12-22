#include "StateUpdate.h"
#include <iostream>
#include <Xml/XmlNodeInterface.h>
#include <Xml/XmlParser2.h>
#include "ObjectUpdater.h"
#include <SafeUtils.h>

namespace Amju
{
GetStateUpdatesReq::GetStateUpdatesReq(const std::string& url) : Ve1Req(url, "get pos updates")
{
}

void GetStateUpdatesReq::HandleResult()
{
  HttpResult res = GetResult();

  if (!res.GetSuccess())
  {
std::cout << "OH NO FAIL! for get pos req: " << res.GetErrorString() << "\n";
    return; // TODO Error state
  }

  std::string str = res.GetString();

//std::cout << "STATE update req result: " << str << "\n";

  // Parse XML, create Object and add to ObjectManager
  PXml xml = ParseXml(str.c_str());

  // Child 0 is timestamp
  PXml p = xml.getChildNode(0);
  if (SafeStrCmp(p.getName(), "now"))
  {
    std::string timestamp = p.getText();
std::cout << "Got new pos update timestamp (from server): " << timestamp << "\n";
    TheObjectUpdater::Instance()->SetTimestampUpdate(timestamp);
  }

  p = xml.getChildNode(1);
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

//std::cout << "Queueing pos for object " << id << " x: " << x << " y: " << y << " z: " << z << "\n";
      // TODO Sanity check ?

      TheObjectUpdater::Instance()->QueueUpdate(id, key, val);
    }
  }
  else
  {
    // Unexpected response from server. Is server reachable ?
    // TODO LOG this error
std::cout << "States update: Didn't find \"states\" tag..\n";
  }
}
}

