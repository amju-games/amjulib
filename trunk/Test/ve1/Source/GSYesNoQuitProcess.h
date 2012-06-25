#ifndef GS_YES_NO_QUIT_PROCESS_H_INCLUDED
#define GS_YES_NO_QUIT_PROCESS_H_INCLUDED

#include <Singleton.h>
#include "GSYesNo.h"

namespace Amju
{
class GSYesNoQuitProcess : public GSYesNo
{
public:
  GSYesNoQuitProcess();
  virtual void OnYes();
};

typedef Singleton<GSYesNoQuitProcess> TheGSYesNoQuitProcess;
}

#endif

