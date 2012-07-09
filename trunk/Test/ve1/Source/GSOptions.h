#ifndef GS_OPTIONS_H_INCLUDED
#define GS_OPTIONS_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
class GSOptions : public GSGui
{
  GSOptions();
  friend class Singleton<GSOptions>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  void OnKeyboard();
};
typedef Singleton<GSOptions> TheGSOptions;
} // namespace
#endif
