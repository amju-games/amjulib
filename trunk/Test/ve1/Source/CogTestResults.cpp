#include <iostream>
#include "CogTestResults.h"
#include "Ve1OnlineReqManager.h"

namespace Amju
{
static const int MAX_CONCURRENT_RESULTS = 10; // max result HTTP reqs in flight

class ReqStoreResult : public Ve1Req
{
public:
  ReqStoreResult(const std::string& url) : Ve1Req(url, "sendresult")
  {
  }

  virtual void OnFailure()
  {
    // Don't show error, we will retry later
  }

  virtual void OnSuccess()
  {
    // Result committed ok: Mark result as committed, so it won't be sent again.

  }
};


bool CogTestResults::Load()
{
  // Load results from file

  return true;
}

bool CogTestResults::Save()
{
  // Save to file immediately

  return true;
}

void CogTestResults::Commit()
{
  // Send each uncommitted result to server.
  
}

void CogTestResults::SendResult(const Result& res)
{
  std::string url = TheVe1ReqManager::Instance()->MakeUrl(SEND_TEST_RESULT);

  url += "&key=" + res.m_key + "&val=" + res.m_val;

std::cout << "Sending this request: " << url << "\n";

  TheVe1ReqManager::Instance()->AddReq(new ReqStoreResult(url), MAX_CONCURRENT_RESULTS); 
}

void CogTestResults::StoreResult(const Result& res)
{
  // Save and keep trying until confirmed

  // TODO TEMP TEST
  SendResult(res);
}
}

