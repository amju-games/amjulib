#ifndef GS_CALENDAR_H_INCLUDED
#define GS_CALENDAR_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
class GSCalendar : public GSGui
{
  GSCalendar();
  friend class Singleton<GSCalendar>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

};
typedef Singleton<GSCalendar> TheGSCalendar;
} // namespace
#endif
