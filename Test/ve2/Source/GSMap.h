#ifndef GS_MAP_H_INCLUDED
#define GS_MAP_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
class GSMap : public GSGui
{
  GSMap();
  friend class Singleton<GSMap>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

};
typedef Singleton<GSMap> TheGSMap;
} // namespace
#endif
