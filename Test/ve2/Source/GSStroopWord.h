#ifndef GS_STROOPWORD_H_INCLUDED
#define GS_STROOPWORD_H_INCLUDED

#include <Singleton.h>
#include "GSStroopBase.h"

namespace Amju 
{
class GSStroopWord : public GSStroopBase
{
  GSStroopWord();
  friend class Singleton<GSStroopWord>;

public:

private:
  void SetTest();

private:
};
typedef Singleton<GSStroopWord> TheGSStroopWord;
} // namespace
#endif
