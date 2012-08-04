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

enum TestId
{
  AMJU_COG_TEST_LETTER_CAN = 1,
  AMJU_COG_TEST_SYMBOL_CAN,
  AMJU_COG_TEST_STROOP_WORD,
  AMJU_COG_TEST_STROOP_COLOUR,
  AMJU_COG_TEST_STROOP_COLOUR_WORD,
  AMJU_COG_TEST_REACTION_TIME
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

private:
  void SendResult(Result*);

private:
  typedef std::vector<Result*> Results;
  Results m_results;
};
typedef Singleton<CogTestResults> TheCogTestResults;
}


#endif

