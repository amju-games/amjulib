#include <AmjuFirst.h>
#include "Ve1Req.h"
#include <Xml/XmlParser2.h>
#include <SafeUtils.h>
#include <StringUtils.h>
#include <iostream>
#include "GSNetError.h"
#include "CreateCollect.h"
#include "HeartCount.h"
#include <AmjuFinal.h>

namespace Amju
{
int Ve1Req::s_criticalErrors = 0;
int Ve1Req::s_nonCriticalErrors = 0;

void Ve1Req::GetNumErrors(int* critical, int* nonCritical)
{
  *critical = s_criticalErrors;
  *nonCritical = s_nonCriticalErrors;
}

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
    s_criticalErrors++;

    ShowError(m_name + ": " + m_errorStr);
std::cout << "NET ERROR AND IS CRITICAL!?!: " << m_errorStr << " - request name: " << m_name << "\n";
  }
  else
  {
    s_nonCriticalErrors++;

    // Non critical... check if errors happen a lot or just sporadically 
std::cout << "NET ERROR but non-critical: " << m_errorStr << " - request name: " << m_name << "\n";
  }
}

void Ve1Req::CheckCollects2d()
{
  int n = m_xml.nChildNode();

  for (int i = 1; i < n; i++)
  {
    PXml p = m_xml.getChildNode(i);
    if (SafeStrCmp(p.getName(), "collect"))
    {
std::cout << "Found collect msg!!\n";
      // format: <collect> <num/> <type/> <id/> </collect>
      PXml ch = p.getChildNode(0);
      if (!SafeStrCmp(ch.getName(), "num"))
      {
        m_errorStr += "bad collect element";
        OnFailure();
      }
      int num = ToInt(ch.getText());
      ChangeHeartCount(num);
    }
  }
}

void Ve1Req::CheckCollects()
{
  int n = m_xml.nChildNode();

  for (int i = 1; i < n; i++)
  {
    PXml p = m_xml.getChildNode(i);
    if (SafeStrCmp(p.getName(), "collect"))
    {
std::cout << "Found collect msg!!\n";
      // format: <collect> <num/> <type/> <id/> </collect>
      PXml ch = p.getChildNode(0);
      if (!SafeStrCmp(ch.getName(), "num"))
      {
        m_errorStr += "bad collect element";
        OnFailure();
      }
      int num = ToInt(ch.getText());
      ch = p.getChildNode(1);
      int type = ToInt(ch.getText());
      ch = p.getChildNode(2);
      int id = ToInt(ch.getText()); 
      for (int j = 0; j < num; j++)
      {
        CreateCollect(type, id);
      }
    }
  }
}

}
