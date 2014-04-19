#ifndef QUIT_HANDLER_H_INCLUDED
#define QUIT_HANDLER_H_INCLUDED

#include <EventListener.h>

namespace Amju
{
class QuitHandler : public EventListener
{
public:
  virtual bool OnQuitEvent();
};
}

#endif

