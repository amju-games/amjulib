#include <AmjuFirst.h>
#include "Timestamp.h"
#include <StringUtils.h>
#include <AmjuFinal.h>

namespace Amju
{
std::string TimestampToString(const Timestamp& t)
{
  return ToString(t.ToSeconds()); 
}

Timestamp StringToTimestamp(const std::string& s)
{
  return Timestamp(ToInt(s));
}

}

