#include "VersionChecker.h"
#include "Updater.h"
#include <iostream>
#include <Xml/XmlParser2.h>
#include <Xml/XmlNodeInterface.h>
#include <SafeUtils.h>

namespace Amju
{
 void VersionChecker::HandleResult()
{
  HttpResult res = GetResult(); // Copy it

  if (res.GetSuccess())
  {
std::cout << "Version check got response...\n";

    const std::string& str = res.GetString();
    PXml xml = ParseXml(str.c_str());

    PXml p = xml.getChildNode(0);

    if (p.getName())
    {
std::cout << " XML: Got child 0, name is " << p.getName() << "\n";
    }
    else
    {
std::cout << " XML: No child 0\n";
    }

    if (SafeStrCmp(p.getName(), "version"))
    {
//#ifdef XML_DEBUG
std::cout << "Found version element\n";
//#endif

      std::string version = p.getText();

      // Only if we get valid version, else just Unwait
      TheUpdater::Instance()->OnServerResponse(version);
    }
    else
    {
std::cout << "Version check got response but no version element! Got this: \"" << str << "\"\n";

      // So don't download if we can't get version info from server...?
      TheUpdater::Instance()->Unwait();
    }
  }
  else
  {
std::cout << "Version check failed! Got this: \"" << res.GetErrorString() << "\"\n";

    // So don't download if we can't get version info from server...?
    TheUpdater::Instance()->Unwait();
  }
}
}


