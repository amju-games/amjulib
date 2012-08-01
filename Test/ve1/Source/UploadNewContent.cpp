#include <iostream>
#include <set>
#include <File.h>
#include <StringUtils.h>
#include <UrlUtils.h>
#include <ObjMesh.h>
#include "Ve1OnlineReqManager.h"
#include "UploadReq.h"

namespace Amju
{
bool CheckCanLoadObj(const std::string pathFile)
{
std::cout << "Attempting to load obj: " << pathFile << "\n";

  std::string path = GetFilePath(pathFile);
  std::string origRoot = File::GetRoot();
  File::SetRoot(path , "/"); // TODO change SetRoot params

  std::string objFile = StripPath(pathFile);

  // TODO Bypass Res Manager so we don't cache the file, we want to reload it.
  // Also, no path is given here so files with the same name in different dirs
  //  won't work!!!!
  bool binary = (GetFileExt(objFile) == "objb");
  RCPtr<ObjMesh> mesh = new ObjMesh;
  bool loaded = mesh->Load(objFile, binary);

  // After loading, revert to original file root
  File::SetRoot(origRoot, "/");

  return loaded;
}


bool UploadObjFile(
  const std::string& objPathAndFilename, 
  const std::string& serverDir, 
  const std::string& assetFilename, const std::string& assetFileHeader, 
  const std::string& dataFilename, const std::string& dataFileHeaderi,
  int* totalNumFiles)
{

  std::string path = GetFilePath(objPathAndFilename);
  std::string origRoot = File::GetRoot();
  File::SetRoot(path , "/"); // TODO change SetRoot params

  std::string objFile = StripPath(objPathAndFilename);

  // TODO Bypass Res Manager so we don't cache the file, we want to reload it.
  // Also, no path is given here so files with the same name in different dirs
  //  won't work!!!!
  bool binary = (GetFileExt(objFile) == "objb");
  RCPtr<ObjMesh> mesh = new ObjMesh;
  bool loaded = mesh->Load(objFile, binary);

  // After loading, revert to original file root
  File::SetRoot(origRoot, "/");

  if (!loaded)
  {
    //SetError("Failed to load obj file " + objFile);
    return false;
  }

std::cout << "Loaded obj file ok!!\n";

  typedef std::set<std::string> StrSet;
  // Pathname + Filename of files to upload for the new Location
  StrSet strs;

  // Make list of .mtl files and textures to upload.
  strs.insert(objPathAndFilename);

  MaterialVec mats;
  mesh->GetMaterials(&mats);
  for (unsigned int i = 0; i < mats.size(); i++)
  {
    const Material* m = mats[i];
std::cout << "Material: " << m->m_name << ", " << m->m_filename << " tex: " << m->m_texfilename << "\n";

    if (!m->m_filename.empty())
    {
      strs.insert(path + "/" + m->m_filename);
    }
    if (!m->m_texfilename.empty())
    {
      strs.insert(path + "/" + m->m_texfilename);
    }
  }
  
  int totalFiles = strs.size() + 2;  // plus one for the asset file, one for data file
  *totalNumFiles = totalFiles;

  // Make an asset file for the Location object, just as a string in mem.
  std::string assetFile = assetFileHeader; //"// Asset file for location " + id + "\r\n// Version\r\n1\r\n";

  //SetError("Uploading " + ToString(totalFiles) + " files, please wait...");

  for (StrSet::iterator it = strs.begin(); it != strs.end(); ++it)
  {
    const std::string& file = *it;

    assetFile += serverDir + "/" + StripPath(file) + "\r\n";

    // TODO Check if file already exists - don't upload if it's the same.
    // DO warn about name clashes, showing files which will not be uploaded and files which will be changed.
std::cout << "UPLOAD THIS FILE: " << file << "\n";

    // TODO Hide from this layer the impl detail that we are sending an HTTP request!!!
    // Just send a message to the server.

    std::string url = TheVe1ReqManager::Instance()->MakeUrl(FILE_UPLOAD);
    url += "&dir=" + serverDir;
    url += "&filename=" + StripPath(file) + "&data=";

    if (FileContentToUrl(file, &url))
    {
      UploadReq* up = new UploadReq(url, StripPath(file));
      // Number of concurrent reqs is number of files
      TheVe1ReqManager::Instance()->AddReq(up, totalFiles);
    }
    else
    {
      //SetError("Failed to make upload request for file " + file);
      return false;
    }
  }

  // Upload asset file
  std::string url = TheVe1ReqManager::Instance()->MakeUrl(FILE_UPLOAD);
  url += "&dir=" + serverDir;
  url += "&filename=" + assetFilename + "&data=" + ToUrlFormat(assetFile);

std::cout << "Uploading asset file: Req=" << url << "\n";

//  url = ToUrlFormat(url);
  TheVe1ReqManager::Instance()->AddReq(new UploadReq(url, assetFilename), totalFiles);

  // Upload location data file
  url = TheVe1ReqManager::Instance()->MakeUrl(FILE_UPLOAD);
  url += "&dir=" + serverDir;
  url += "&filename=" + dataFilename + "&data=" +  
    ToUrlFormat("// version\r\n1\r\n// obj file\r\n" + serverDir + "/" + objFile);
  // TODO Use dataFile contents passed in

std::cout << "Uploading data file: Req=" << url << "\n";

//  url = ToUrlFormat(url);
  TheVe1ReqManager::Instance()->AddReq(new UploadReq(url, dataFilename), totalFiles);

  return true;
}


}

