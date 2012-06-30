#ifndef GS_STROOPCOLOUR_H_INCLUDED
#define GS_STROOPCOLOUR_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
class GSStroopColour : public GSGui
{
  GSStroopColour();
  friend class Singleton<GSStroopColour>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

};
typedef Singleton<GSStroopColour> TheGSStroopColour;
} // namespace
#endif
