#ifndef GS_STROOPWORD_H_INCLUDED
#define GS_STROOPWORD_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
class GSStroopWord : public GSGui
{
  GSStroopWord();
  friend class Singleton<GSStroopWord>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

};
typedef Singleton<GSStroopWord> TheGSStroopWord;
} // namespace
#endif
