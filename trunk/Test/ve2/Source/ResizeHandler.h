#ifndef RESIZE_HANDLER_H_INCLUDED
#define RESIZE_HANDLER_H_INCLUDED

#include <EventListener.h>

namespace Amju
{
struct ResizeHandler : public EventListener
{
  virtual bool OnResizeEvent(const ResizeEvent&);
};
}

#endif

