#include <AmjuFirst.h>
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
#include "ObjectUpdater.h"
#include "GSNetError.h"
#include <AmjuFinal.h>

namespace Amju
{
PosUpdateReq::PosUpdateReq(const std::string& url) : Ve1Req(url, "pos update req") 
{
  m_critical = false;
}

void PosUpdateReq::OnSuccess()
{
  // Child 0 is timestamp
//  PXml p = m_xml.getChildNode(0);
//  Assert(SafeStrCmp(p.getName(), "now"));
//  std::string timestamp = p.getText();
//std::cout << "Got new pos update timestamp: " << timestamp << "\n";

  TheObjectUpdater::Instance()->SetTimestampPos(m_timestamp);

  PXml p = m_xml.getChildNode(1);
  if (SafeStrCmp(p.getName(), "objs"))
  {
#ifdef XML_DEBUG
std::cout << "found objs element\n";
#endif

    int numObjs = p.nChildNode();

#ifdef XML_DEBUG
if (numObjs > 0)
{
  std::cout << "PosUpdateReq: got " << numObjs << " positions.\n";
}
#endif

    for (int i = 0; i < numObjs; i++)
    {
      PXml obj = p.getChildNode(i);

#ifdef XML_DEBUG
std::cout << "Obj " << i << ": ";
#endif

      int id = ToInt(obj.getChildNode(0).getText());
      float x = ToFloat(obj.getChildNode(1).getText());
      float y = ToFloat(obj.getChildNode(2).getText());
      float z = ToFloat(obj.getChildNode(3).getText());
      int location = atoi(obj.getChildNode(4).getText());

//std::cout << "##@@## Got msg from server, Queueing pos for object " << id << " location: " << location << " x: " << x << " y: " << y << " z: " << z << "\n";

      TheObjectUpdater::Instance()->QueueUpdatePos(id, Vec3f(x, y, z), location);
    }
  }
  else
  {
    // Unexpected response from server. Is server reachable ?
    // TODO LOG this error
    ShowError("Pos update: Didn't find \"objs\" tag in response");
  }
}

}

