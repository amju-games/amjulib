#ifndef COG_TEST_RESULTS_H_INCLUDED
#define COG_TEST_RESULTS_H_INCLUDED

#include <string>
#include <Singleton.h>

namespace Amju
{
class Result
{
public:
  Result(const std::string& niceName, const std::string& key, const std::string& val) :  
    m_niceName(niceName), m_key(key), m_val(val), m_confirmed(false)
  {
//    m_testId = <get current test id or session id>
  }

private:
  friend class CogTestResults;

  std::string m_niceName;
  std::string m_key;
  std::string m_val; 

  bool m_confirmed;
};

// Send results to server: until we get confirmation that the result was received, we should
//  store locally and keep retrying.
// We may also want to display the results to the player, so need to retrieve some results.
// We may want to show the player ALL his/her results, which would need to be retrieved from the server.
class CogTestResults : public NonCopyable
{
public:
  bool Load();
  bool Save();

  void StoreResult(const Result&);

  // Call to attempt to send all outstanding results to server
  void Commit();

private:
  void SendResult(const Result&);

private:
};
typedef Singleton<CogTestResults> TheCogTestResults;
}


#endif

