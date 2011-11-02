#include "RB.h"

namespace Amju
{
RB::RB()
{
  static int id = 0;
  m_id = id++;
  m_atRest = false;
}

void RB::SetResting(bool isResting)
{
  m_atRest = isResting;
}

bool RB::IsResting() const
{
  return m_atRest;
}
}

