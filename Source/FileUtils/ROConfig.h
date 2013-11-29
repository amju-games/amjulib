#ifndef R_O_CONFIG_H_INCLUDED
#define R_O_CONFIG_H_INCLUDED

#include <ConfigFile.h>

namespace Amju
{
void SetROConfigFilename(const std::string& s);

// Read-only config: don't save to it, use Game Config File for settable values.
// This file could be downloaded from server, but is not written to by the client.
ConfigFile* ROConfig();
}

#endif
