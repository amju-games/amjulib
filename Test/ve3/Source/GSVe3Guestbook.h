#ifndef GS_VE3GUESTBOOK_H_INCLUDED
#define GS_VE3GUESTBOOK_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
class GSVe3Guestbook : public GSGui
{
  GSVe3Guestbook();
  friend class Singleton<GSVe3Guestbook>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

};
typedef Singleton<GSVe3Guestbook> TheGSVe3Guestbook;
} // namespace
#endif
