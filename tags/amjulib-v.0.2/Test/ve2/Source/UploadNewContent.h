#ifndef UPLOAD_NEW_CONTENT_H_INCLUDED
#define UPLOAD_NEW_CONTENT_H_INCLUDED

#include <string>

namespace Amju
{
// Locations and Game Objects have associated data files - these functions upload the files.
// For obj meshes, we look for mtl and image files, and upload these too.
// All asset files are listed in one text file for the object.

// TODO Error codes

// Check this OBJ file is ok. Full path and filename - presumably from GUI file browser.
bool CheckCanLoadObj(const std::string objFullPathAndFilename);

bool UploadObjFile(
  const std::string& objPathAndFilename,
  const std::string& serverDir,
  const std::string& assetFilename, const std::string& assetFileHeader,
  const std::string& dataFilename, const std::string& dataFileHeader,
  int* totalNumFiles);
}

#endif

