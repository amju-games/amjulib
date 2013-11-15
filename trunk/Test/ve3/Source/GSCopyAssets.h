#ifndef GS_COPYASSETS_H_INCLUDED
#define GS_COPYASSETS_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
class GSCopyAssets : public GSGui
{
  GSCopyAssets();
  friend class Singleton<GSCopyAssets>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

};
typedef Singleton<GSCopyAssets> TheGSCopyAssets;
} // namespace
#endif
