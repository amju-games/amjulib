#ifndef COG_TEST_RESULTS_H_INCLUDED
#define COG_TEST_RESULTS_H_INCLUDED

#include <string>
#include <vector>
#include <Singleton.h>
#include <AmjuTime.h>
#include "Ve1OnlineReqManager.h"

namespace Amju
{
class File;

// This is the full list of all available tests.
enum TestId
{
  AMJU_COG_TEST_LETTER_CAN = 1,
  AMJU_COG_TEST_SYMBOL_CAN,
  AMJU_COG_TEST_STROOP_WORD,
  AMJU_COG_TEST_STROOP_COLOUR,
  AMJU_COG_TEST_STROOP_COLOUR_WORD,
  AMJU_COG_TEST_REACTION_TIME,
  AMJU_COG_TEST_TRAIL_MAKING,
  AMJU_COG_TEST_TRAIL_MAKING_ALTERNATE,

  AMJU_COG_TEST_MAX
};

// For each test ID above, the number of test results we expect to get.
// If we have that many results, the test is complete.
const int EXPECTED_NUM_RESULTS_FOR_TEST[] = 
{
  -1, // 
  3,  // AMJU_COG_TEST_LETTER_CAN = 1,
  3,  // AMJU_COG_TEST_SYMBOL_CAN,
  2,  // AMJU_COG_TEST_STROOP_WORD,
  2,  // AMJU_COG_TEST_STROOP_COLOUR,
  2,  // AMJU_COG_TEST_STROOP_COLOUR_WORD,
  3,  // AMJU_COG_TEST_REACTION_TIME
  3,  // AMJU_COG_TEST_TRAIL_MAKING   
  3,  // AMJU_COG_TEST_TRAIL_MAKING_ALTERNATE   
};

class Result
{
public:
  Result() : m_testId(-1), m_key("none"), m_val("none"), m_committed(false), m_timestamp(Time::Now())
  {
    m_sessionId = TheVe1ReqManager::Instance()->GetSessionId();
  }

  Result(int testId, const std::string& key, const std::string& val) :  
    m_testId(testId), m_key(key), m_val(val), m_committed(false), m_timestamp(Time::Now())
  {
    m_sessionId = TheVe1ReqManager::Instance()->GetSessionId();
  }

  bool Load(File*);
  bool Save(File*);

  bool NoSessionId() const { return m_sessionId.empty(); }

  const std::string& GetKey() const { return m_key; }
  const std::string& GetVal() const { return m_val; }
  Time GetTimestamp() const { return m_timestamp; }

private:
  friend class CogTestResults;
  friend class ReqStoreResult;

  std::string m_sessionId;
  int m_testId; 
  std::string m_key;
  std::string m_val; 

  bool m_committed;

  Time m_timestamp;
};

typedef std::vector<Result*> Results;

// Send results to server: until we get confirmation that the result was received, we should
//  store locally and keep retrying.
// We may also want to display the results to the player, so need to retrieve some results.
// We may want to show the player ALL his/her results, which would need to be retrieved from the server.
class CogTestResults : public NonCopyable
{
  CogTestResults();
  friend class Singleton<CogTestResults>;

public:
  ~CogTestResults();

  bool Load();
  bool Save();

  void StoreResult(Result*);

  // Call to attempt to send all outstanding results to server
  void Commit();

  // Returns Results for the given test type.
  // NB This will only return historical results stored locally, it doesn't pull down from server.
  Results GetResultsForTestType(TestId);

  // Returns Results for the given date (timestamp is rounded down).
  // NB This will only return historical results stored locally, it doesn't pull down from server.
  Results GetResultsForDate(Time);

  // Return true if we have the expected number of test results for the given date. 
  bool HaveGotAllResultsForDate(Time);

  // For the given date, return the number of results for the given test ID.
  int GetNumResults(Time, TestId);

  // For the given date, is the given test done, i.e. do we have the expected number of results.
  bool IsTestComplete(Time, TestId);

private:
  void SendResult(Result*);

private:
  Results m_results;
};
typedef Singleton<CogTestResults> TheCogTestResults;
}


#endif

