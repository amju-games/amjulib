#include <Singleton.h>
#include "GSLikertQuestions.h"

namespace Amju
{
class GSSatisfactionWithLifeScale : public GSLikertQuestions
{
public:
  GSSatisfactionWithLifeScale();
};

typedef Singleton<GSSatisfactionWithLifeScale> TheGSSatisfactionWithLifeScale;
}
