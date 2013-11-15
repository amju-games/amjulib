#ifndef R_O_CONFIG_H_INCLUDED
#define R_O_CONFIG_H_INCLUDED

#include <ConfigFile.h>

namespace Amju
{
// Read-only config: don't save to it, use Game Config File.
// Could be downloaded from server.
ConfigFile* ROConfig();
}

#endif
