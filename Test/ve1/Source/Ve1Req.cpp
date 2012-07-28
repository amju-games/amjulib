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
      m_timestamp = p.getText();

      // TODO More checking
      success = true;
    }
    else
    {
      // TODO Get HTTP code
      m_errorStr = m_name + ": Didn't get time stamp in result: " + str;
    }

    if (m_xml.nChildNode() > 1) 
    {
      p = m_xml.getChildNode(1);
      if (SafeStrCmp(p.getName(), "error"))
      {
        m_errorStr = p.getText();
        success = false;
      }
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

  if (m_critical)
  {
    ShowError(m_errorStr);
  }
  else
  {
    // Non critical... check if errors happen a lot or just sporadically 
std::cout << "NET ERROR but non-critical...\n";
  }
}
}
