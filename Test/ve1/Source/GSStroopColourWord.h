#ifndef GS_STROOPCOLOURWORD_H_INCLUDED
#define GS_STROOPCOLOURWORD_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
class GSStroopColourWord : public GSGui
{
  GSStroopColourWord();
  friend class Singleton<GSStroopColourWord>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

};
typedef Singleton<GSStroopColourWord> TheGSStroopColourWord;
} // namespace
#endif
