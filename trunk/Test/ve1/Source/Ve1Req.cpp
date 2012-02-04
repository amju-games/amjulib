#include "Ve1Req.h"
#include <Xml/XmlParser2.h>
#include <SafeUtils.h>
#include <iostream>
#include "GSNetError.h"

namespace Amju
{
void Ve1Req::HandleResult()
{
  m_errorStr.clear();

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
      // TODO Get HTTP code
      m_errorStr = m_name + ": Didn't get time stamp in result: " + str;
    }

  }
  else
  {
    m_errorStr = res.GetErrorString();
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

  ShowError(m_errorStr);
}
}
