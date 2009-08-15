#ifndef ALPHA_BMP_LOADER_H
#define ALPHA_BMP_LOADER_H

#include "ResourceManager.h"

namespace Amju
{
// Handler for bitmaps with alpha. The alpha channel is in a companion
// bitmap.
// E.g. resource name is "image.bmpa"
// RGB image is "image.bmp". Greyscale alpha is "image-a.bmp"
Resource* BmpALoader(const std::string& resName);

}

#endif
