#include "PosUpdate.h"
#include <Game.h>
#include <GameObject.h>
#include "Ve1Req.h"
#include <iostream>
#include <Xml/XmlParser2.h>
#include <Xml/XmlNodeInterface.h>
#include <SafeUtils.h>

namespace Amju
{
class GetPosReq : public Ve1Req
{
public:
  GetPosReq(const std::string& url) : Ve1Req(url, "get pos req") {}

  virtual void HandleResult()
  {

    HttpResult res = GetResult();

    if (!res.GetSuccess())
    {
std::cout << "OH NO FAIL! for get pos req: " << res.GetErrorString() << "\n";
      return; // TODO Error state
    }

    std::string str = res.GetString();

    // Parse XML, create Object and add to ObjectManager
    PXml xml = ParseXml(str.c_str());

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
        int x = atof(obj.getChildNode(1).getText());
        int y = atof(obj.getChildNode(2).getText());
        int z = atof(obj.getChildNode(3).getText());

std::cout << "Updating pos for object " << id << " x: " << x << " y: " << y << " z: " << z << "\n";
        // TODO Sanity check ?
        GameObject* go = TheGame::Instance()->GetGameObject(id);
        if (go)
        {
          go->SetPos(Vec3f(x, y, z));
        }
        else
        {
std::cout << "Unexpected game object ID " << id << " returned from get pos req.\n";
        }
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

void PosUpdate()
{
  // Iterate over GOs. Get update period, work out if it's time to query for pos change.
  // (New objects should update pos ASAP.)
 
  // Get pos updates periodically.
   
}

}

