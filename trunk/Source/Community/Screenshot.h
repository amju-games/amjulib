/*
Amju Games source code (c) Copyright Jason Colman 2000-2006
*/

#ifndef SCREENSHOT_H_INCLUDED
#define SCREENSHOT_H_INCLUDED

#include <string>

namespace Amju
{
// Writes file
// scale is number of times smaller to make the image
void SaveScreenshot(const std::string& filename, int scale);

// Reads file and sends to server
// url has image data added to it
void UploadScreenshot(const std::string& filename, const std::string& url);
}

#endif
