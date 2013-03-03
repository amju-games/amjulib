#ifndef GS_YES_NO_QUIT_SESSION_H_INCLUDED
#define GS_YES_NO_QUIT_SESSION_H_INCLUDED

#include <Singleton.h>
#include "GSYesNo.h"

namespace Amju
{
class GSYesNoQuitSession : public GSYesNo
{
public:
  GSYesNoQuitSession();
  virtual void OnYes();
};

typedef Singleton<GSYesNoQuitSession> TheGSYesNoQuitSession;
}

#endif

