#ifndef COG_TEST_NAG_H_INCLUDED
#define COG_TEST_NAG_H_INCLUDED

#include <Singleton.h>

namespace Amju
{
// Handle asking player to take cog test, if it is scheduled this session.
// Allow player to say no, but then ask again after a time delay.
class CogTestNag : public NonCopyable
{
  CogTestNag();
  friend class Singleton<CogTestNag>;

public:
  void Update(); 
};

void OnCogTestStopPartWayThrough();

void NagReset();

typedef Singleton<CogTestNag> TheCogTestNag;
}

#endif