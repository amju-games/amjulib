// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026
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
  
// Convert to/from 'encoded' form of string. This is the same as URL format, but without
//  the % signs. E.g. Encode(" ") -> "20"; Decode("20") -> " "
std::string EncodeStr(const std::string& plainMsg);
std::string DecodeStr(const std::string& encodedMsg);
}

#endif


