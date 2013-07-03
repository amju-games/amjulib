#ifndef GS_STROOPCOLOURWORD_H_INCLUDED
#define GS_STROOPCOLOURWORD_H_INCLUDED

#include <Singleton.h>
#include "GSStroopBase.h"

namespace Amju 
{
class GSStroopColourWord : public GSStroopBase
{
  GSStroopColourWord();
  friend class Singleton<GSStroopColourWord>;

public:

private:
  void SetTest();

private:
  bool m_isFinished;
};
typedef Singleton<GSStroopColourWord> TheGSStroopColourWord;
} // namespace
#endif
