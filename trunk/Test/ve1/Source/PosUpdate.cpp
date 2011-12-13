#include "PosUpdate.h"
#include <Game.h>
#include <GameObject.h>
#include "Ve1Req.h"
#include <iostream>
#include <Xml/XmlParser2.h>
#include <Xml/XmlNodeInterface.h>
#include <SafeUtils.h>
#include <Timer.h>
#include "Ve1OnlineReqManager.h"
#include "Ve1Object.h"

namespace Amju
{
static std::string timestamp = "1";

class PosUpdateReq : public Ve1Req
{
public:
  PosUpdateReq(const std::string& url) : Ve1Req(url, "pos update req") {}

  virtual void HandleResult()
  {

    HttpResult res = GetResult();

    if (!res.GetSuccess())
    {
std::cout << "OH NO FAIL! for get pos req: " << res.GetErrorString() << "\n";
      return; // TODO Error state
    }

    std::string str = res.GetString();
std::cout << "Pos update req result: " << str << "\n";

    // Parse XML, create Object and add to ObjectManager
    PXml xml = ParseXml(str.c_str());

    // Child 0 is timestamp
    PXml p = xml.getChildNode(0);
    if (SafeStrCmp(p.getName(), "now"))
    {
      timestamp = p.getText();
std::cout << "Got new pos update timestamp: " << timestamp << "\n";
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
        int x = atof(obj.getChildNode(1).getText());
        int y = atof(obj.getChildNode(2).getText());
        int z = atof(obj.getChildNode(3).getText());

std::cout << "Updating pos for object " << id << " x: " << x << " y: " << y << " z: " << z << "\n";
        // TODO Sanity check ?
        GameObject* go = TheGame::Instance()->GetGameObject(id);
        if (go)
        {
          Ve1Object* ve1Obj = dynamic_cast<Ve1Object*>(go);
          if (ve1Obj)
          {
            ve1Obj->MoveTo(Vec3f(x, y, z));
          }
          else
          {
            go->SetPos(Vec3f(x, y, z));
          }
        }
        else
        {
          // This could happen if a new object has just appeared in the world and we haven't 
          //  created it yet (e.g. haven't downloaded all assets).
std::cout << "Unexpected game object ID " << id << " returned from get pos req.\n";
          // Don't update timestamp, so we can get the pos again when the object has been created
        }
      }
    }
    else
    {
      // Unexpected response from server. Is server reachable ?
      // TODO LOG this error
std::cout << "Pos update: Didn't find \"objs\" tag..\n";
    }
  }
};

void PosUpdate()
{
  // Get pos updates periodically.
  // TODO Only get objects whose position has changed since last check ? 
  static float elapsed = 0;
  elapsed += TheTimer::Instance()->GetDt();

  // TODO These poll periods should depend on network latency/bandwidth
  static const float POS_UPDATE_PERIOD = 10.0f; // TODO CONFIG
  if (elapsed > POS_UPDATE_PERIOD)
  {
std::cout << "Creating new pos update req...\n";

    elapsed = 0;

    std::string url = TheVe1ReqManager::Instance()->MakeUrl(POS_UPDATE_REQ);
    url += "&time=" + timestamp;

    // TODO Timestamp in query
    std::cout << "URL: " << url << "\n";

    TheVe1ReqManager::Instance()->AddReq(new PosUpdateReq(url));
  }
}

}

