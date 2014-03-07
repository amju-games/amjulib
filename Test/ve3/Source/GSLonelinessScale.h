#include <Singleton.h>
#include "GSLikertQuestions.h"

namespace Amju
{
class GSLonelinessScale : public GSLikertQuestions
{
public:
  GSLonelinessScale();
};

typedef Singleton<GSLonelinessScale> TheGSLonelinessScale;
}
