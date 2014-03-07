#include <Singleton.h>
#include "GSLikertQuestions.h"

namespace Amju
{
class GSSatisfactionWithLifeScale : public GSLikertQuestions
{
public:

};

typedef Singleton<GSSatisfactionWithLifeScale> TheGSSatisfactionWithLifeScale;
}
