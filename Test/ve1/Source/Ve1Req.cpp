#include "Ve1Req.h"
#include <Xml/XmlParser2.h>
#include <SafeUtils.h>
#include <iostream>

namespace Amju
{
void Ve1Req::HandleResult()
{
  bool success = false;

  const HttpResult& res = GetResult();
  if (res.GetSuccess())
  {
    const std::string& str = res.GetString();
    m_xml = ParseXml(str.c_str());

    // For all responses, Child 0 is current server time

    PXml p = m_xml.getChildNode(0);
    if (SafeStrCmp(p.getName(), "now"))
    {
      // TODO More checking
      success = true;
    }
    else
    {
std::cout << m_name << ": Didn't get time stamp in result\n";
    }

  }

  if (success)
  {
    OnSuccess();
  }
  else
  {
    OnFailure();
  }
}

void Ve1Req::OnFailure()
{
    // TODO  Log error msg
    // TODO  Check for timeout etc
  // TODO Display error msg
}
}
