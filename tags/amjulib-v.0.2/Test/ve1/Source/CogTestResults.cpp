#include <iostream>
#include <StringUtils.h>
#include <File.h>
#include <TimePeriod.h>
#include "CogTestResults.h"
#include "Ve1OnlineReqManager.h"

namespace Amju
{
static const int MAX_CONCURRENT_RESULTS = 100; // max result HTTP reqs in flight

static const char* COGTEST_FILENAME = "cogtest.txt";

class ReqStoreResult : public Ve1Req
{
public:
  ReqStoreResult(const std::string& url, Result* res) : Ve1Req(url, "sendresult"), m_res(res)
  {
  }

  virtual void OnFailure()
  {
    // Don't show error, we will retry later
std::cout << "Store result req FAILED! for this res: " 
  << m_res->m_sessionId  << "/" 
  << m_res->m_testId     << "/" 
  << m_res->m_key        << "/" 
  << m_res->m_val        << ".. will retry later.\n";
  }

  virtual void OnSuccess()
  {
    // Result committed ok: Mark result as committed, so it won't be sent again.
    m_res->m_committed = true;

    //CheckCollects();
    CheckCollects2d();
  }

private:
  Result* m_res;
};

bool Result::Load(File* f)
{
  if (!f->GetDataLine(&m_sessionId))
  {
    f->ReportError("Expected session ID in cog test result");
    return false;
  }

  if (!f->GetInteger(&m_testId))
  {
    f->ReportError("Expected test ID in cog test result");
    return false;
  }

  if (!f->GetDataLine(&m_key))
  {
    f->ReportError("Expected key in cog test result");
    return false;
  }
 
  if (!f->GetDataLine(&m_val))
  {
    f->ReportError("Expected val in cog test result");
    return false;
  }

  int c;
  if (!f->GetInteger(&c))
  {
    f->ReportError("Expected committed flag in cog test result");
    return false;
  }
  m_committed = (c == 1);

  if (!m_timestamp.Load(f))
  {
    return false;
  }

  return true;
}

bool Result::Save(File* f)
{
  f->WriteComment("// Result: sessionId/testId/key/val/committed/timestamp:");
  f->Write(m_sessionId);
  f->WriteInteger(m_testId);
  f->Write(m_key);
  f->Write(m_val);
  f->WriteInteger(m_committed ? 1 : 0);
  m_timestamp.Save(f);

  return true;
}

CogTestResults::CogTestResults()
{
  if (!Load())
  {
    // What to do ?! We have lost cog test results!! :-(
    // (or none saved yet!)
  }
}

CogTestResults::~CogTestResults()
{
  Save();
}

bool CogTestResults::Load()
{
  File f;
  if (!f.OpenRead(COGTEST_FILENAME))
  {
    return false;
  }

  int numResults = 0;
  if (!f.GetInteger(&numResults))
  {
    f.ReportError("Expected num cog test results");
    return false;
  }

  m_results.reserve(numResults);

  for (int i = 0; i < numResults; i++)
  {
    Result* res = new Result;
    if (!res->Load(&f))
    {
      return false;
    }
    m_results.push_back(res);
  }

  Commit(); // make sure results are sent
  return true;
}

Results CogTestResults::GetResultsForDate(Time testDate)
{
  testDate.RoundDown(TimePeriod(ONE_DAY_IN_SECONDS));
  Results r;

  for (unsigned int i = 0; i < m_results.size(); i++)
  {
    Result* res = m_results[i];
    Time t = res->m_timestamp;
    t.RoundDown(TimePeriod(ONE_DAY_IN_SECONDS));
    if (t == testDate)
    {
      r.push_back(res);
    }
  }

  return r;
}

bool CogTestResults::Save()
{
  // Save to file immediately
  File f;
  if (!f.OpenWrite(COGTEST_FILENAME))
  {
    // TODO error msg!
    return false;
  }

  if (!f.WriteInteger(m_results.size()))
  {
    f.ReportError("Failed to write to cog test results file");
    return false;
  }

  int size = m_results.size();
  for (int i = 0; i < size; i++)
  {
    Result* res = m_results[i];
    if (!res->Save(&f))
    {
      // Crap, we are going to lose cog test data :-(
      // TODO Show emergency error message

      return false;
    }
  }

  return true;
}

void CogTestResults::Commit()
{
  // Send all uncommitted results to server.
  int size = m_results.size();
  for (int i = 0; i < size; i++)
  {
    Result* res = m_results[i];
    if (!res->m_committed)
    {
      SendResult(res);
    }
  } 
}

void CogTestResults::SendResult(Result* res)
{
  std::string url = TheVe1ReqManager::Instance()->MakeUrl(SEND_TEST_RESULT);

  url += "&test_type=" + ToString(res->m_testId) + 
    "&research_sessionid=" + res->m_sessionId + 
    "&key=" + res->m_key + 
    "&val=" + res->m_val;

std::cout << "Sending this request: " << url << "\n";

  TheVe1ReqManager::Instance()->AddReq(new ReqStoreResult(url, res), MAX_CONCURRENT_RESULTS); 
}

void CogTestResults::StoreResult(Result* res)
{
  // Save and keep trying until confirmed
  m_results.push_back(res);
  Save();

  Commit();
}
}

