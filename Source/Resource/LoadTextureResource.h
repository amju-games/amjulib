#ifndef LOAD_TEXTURE_RESOURCE_H
#define LOAD_TEXTURE_RESOURCE_H

namespace Amju
{
class Texture;
class File;

// Convenience function: read a line of text from a file, get the Resource
// and return it, cast to a Texture*
Texture* LoadTextureResource(File*); 
}

#endif
