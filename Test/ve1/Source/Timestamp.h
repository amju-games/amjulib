#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <AmjuTime.h>

namespace Amju
{
// Use this type for database timestamps
typedef Time Timestamp;

std::string TimestampToString(const Timestamp&);

Timestamp StringToTimestamp(const std::string&);
}

#endif

