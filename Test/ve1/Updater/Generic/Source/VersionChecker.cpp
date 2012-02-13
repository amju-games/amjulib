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
    m_updater->Report("Version check got response...\n");

    const std::string& str = res.GetString();
    PXml xml = ParseXml(str.c_str());

    PXml p = xml.getChildNode(0);

    if (p.getName())
    {
      m_updater->Report(std::string("XML: Got child 0, name is " +
        std::string(p.getName()) + "\n").c_str());
    }
    else
    {
      m_updater->Report("XML: No child 0\n");
    }

    if (SafeStrCmp(p.getName(), "version"))
    {
      m_updater->Report("Found version element\n");

      std::string version = p.getText();

      // Only if we get valid version, else just Unwait
      m_updater->OnServerResponse(version);
    }
    else
    {
      m_updater->Report(std::string("Version check got response but no version element! Got this: \"" +
        str + "\"\n").c_str());

      // So don't download if we can't get version info from server...?
      m_updater->Unwait();
    }
  }
  else
  {
    m_updater->Report(std::string("Version check failed! Got this error: \"" + 
      res.GetErrorString() + "\"\n").c_str());

    // So don't download if we can't get version info from server...?
    m_updater->Unwait();
  }
}
}


