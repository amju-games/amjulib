#ifndef GS_STROOPCOLOUR_H_INCLUDED
#define GS_STROOPCOLOUR_H_INCLUDED

#include <Singleton.h>
#include "GSStroopBase.h"

namespace Amju 
{
class GSStroopColour : public GSStroopBase
{
  GSStroopColour();
  friend class Singleton<GSStroopColour>;

public:
  virtual void Draw2d();
  virtual void OnActive();
  virtual void OnDeactive();

  void ResetTest(); 

private:
  void SetTest();

private:
  Colour m_goodColour;
  RCPtr<GuiElement> m_colourRect;
};
typedef Singleton<GSStroopColour> TheGSStroopColour;
} // namespace
#endif
