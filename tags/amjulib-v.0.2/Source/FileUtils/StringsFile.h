#ifndef STRINGS_FILE_H_INCLUDED
#define STRINGS_FILE_H_INCLUDED

#include <StringUtils.h>
#include "File.h"

namespace Amju
{
// Load file consisting only of newline-separated strings
bool LoadStrings(const std::string& filename, Strings* result);

// Write strings to separate lines of a file (truncates)
bool SaveStrings(const std::string& filename, const Strings& strs);

}

#endif

