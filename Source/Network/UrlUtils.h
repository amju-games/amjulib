/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: UrlUtils.h,v $
Revision 1.5  2007/01/03 09:05:05  jay
Add Overrides ofToUrlFormat

Revision 1.4  2006/12/04 21:49:38  jay
Added funcs to strip data from URL

Revision 1.3  2006/10/27 22:23:44  jay
Merge improvements from Pool, for HTTP online stuff.

Revision 1.1.2.2  2006/07/27 08:42:46  jay
Added ToUrlFormat(), to correctly format all characters in a URL.

Revision 1.1.2.1  2005/10/21 17:51:59  jay
All URL-manipulation functions moved here; copied over from Uptime Overlord.

Revision 1.1  2005/01/03 11:08:55  jay
Added to Uptime Overlord project

*/

#ifndef AMJU_URL_UTILS_H_INCLUDED
#define AMJU_URL_UTILS_H_INCLUDED

#include <string>
#include "BinaryData.h"

namespace Amju
{
std::string GetServerNameFromUrl(const std::string& url);

std::string GetPathFromUrl(const std::string& url);

int GetPortFromUrl(const std::string& url);

// Replace spaces with %20, etc.
std::string ToUrlFormat(const std::string& s);

// Convert binary data (e.g. image file to upload) to uploadable format.
std::string ToUrlFormat(const BinaryData& d);

// Convert binary data in array
// Give number of bytes so '\0' can be included in the data.
std::string ToUrlFormat(const unsigned char* const data, int numbytes);

std::string GetDataFromUrl(const std::string& url);

std::string StripDataFromUrl(const std::string& url);

// Load contents of file, convert to string to append to URL for a POST.
// NB File path is absolute, no file root is prepended.
bool FileContentToUrl(const std::string& filename, std::string* result);
}

#endif


