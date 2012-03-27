#include <iostream>
#include <Game.h>
#include <StringUtils.h>
#include <ObjMesh.h>
#include <ResourceManager.h>
#include <UrlUtils.h>
#include <SafeUtils.h>
#include <ConfigFile.h>
#include <Directory.h>
#include "GSNewLocation.h"
#include "GSEdit.h"
#include "Ve1OnlineReqManager.h"
#include "LocalPlayer.h"

namespace Amju
{
static const char* LOCATION_PATH_KEY = "last_location_mesh_path";

static void OnOKNew()
{
  TheGSNewLocation::Instance()->OnOKButton();
}

class ReqNewLocId : public Ve1Req
{
public:
  ReqNewLocId(const std::string& url) : Ve1Req(url, "new location id") { }

  virtual void OnSuccess()
  {
    PXml p = m_xml.getChildNode(1);
    if (SafeStrCmp(p.getName(), "id"))
    {
#ifdef XML_DEBUG
std::cout << "Found id element\n";
#endif

      std::string id = p.getText();
std::cout << "Got new location ID from server! " << id << "\n";

      TheGSNewLocation::Instance()->OnNewIdResponse(id);
    }
    else
    {
      OnFailure();
    }
  }

  virtual void OnFailure()
  {
std::cout << "FAILED to get new location ID.\n";
    TheGSNewLocation::Instance()->OnNewIdResponse("");
  }
};

class CreateNewLocationReq : public Ve1Req
{
public:
  CreateNewLocationReq(const std::string& url) : Ve1Req(url, "create location") { } 

  virtual void OnSuccess()
  {
std::cout << "Create new location succeeded!\n";

    TheGSNewLocation::Instance()->OnLocationCreated();
  }

  virtual void OnFailure()
  {
std::cout << "Create new location FAILED!\n";


  const HttpResult& res = GetResult();

std::cout << "RESULT: " << res.GetString() << "\n";

std::cout << "ERROR: " << res.GetErrorString() << "\n";

std::cout << "Error from Ve1Req: " << m_errorStr << "\n";

    TheGSNewLocation::Instance()->SetError("Failed to create new location");
    // TODO Error msg
  }
};

GSNewLocation::GSNewLocation()
{
  m_uploadedFiles = 0;
  m_totalFiles = 0;
}

void GSNewLocation::SetError(const std::string& str)
{
  GuiText* text = dynamic_cast<GuiText*>(m_gui->GetElementByName("error"));
  Assert(text);
  text->SetText(str);
}

void GSNewLocation::OnOKButton()
{
  // Save path/file in prefs.

  // Try loading .obj file. Temporarily set File Root to path to .obj and other files.
  GuiText* text = dynamic_cast<GuiText*>(m_gui->GetElementByName("obj_file"));
  Assert(text);
  std::string pathFile = text->GetText();

std::cout << "Attempting to load Location obj: " << pathFile << "\n";

  std::string path = GetFilePath(pathFile);
  std::string origRoot = File::GetRoot();
  File::SetRoot(path , "/"); // TODO change SetRoot params

  m_objFile = StripPath(pathFile);

  ObjMesh* mesh = (ObjMesh*)TheResourceManager::Instance()->GetRes(m_objFile);

  // After loading, revert to original file root
  File::SetRoot(origRoot, "/");

  if (!mesh)
  {
    SetError("Failed to load obj file " + m_objFile);
    return;
  }

std::cout << "Loaded obj file ok!!\n";

  // Remember this path
  static GameConfigFile* config = TheGameConfigFile::Instance();
  config->Set(LOCATION_PATH_KEY, pathFile);

  // Make list of .mtl files and textures to upload.
  m_strs.clear(); 
  m_strs.insert(pathFile);

  MaterialVec mats;
  mesh->GetMaterials(&mats);
  for (unsigned int i = 0; i < mats.size(); i++)
  {
    const Material* m = mats[i];
std::cout << "Material: " << m->m_name << ", " << m->m_filename << " tex: " << m->m_texfilename << "\n";

    if (!m->m_filename.empty())
    {
      m_strs.insert(path + "/" + m->m_filename);
    }
    if (!m->m_texfilename.empty())
    {
      m_strs.insert(path + "/" + m->m_texfilename);
    }
  }

  RequestNewId();
}

void GSNewLocation::RequestNewId()
{
  // Request a new location ID. Use this for asset file and data file.
  std::string url = TheVe1ReqManager::Instance()->MakeUrl(GET_NEW_LOCATION_ID);
  TheVe1ReqManager::Instance()->AddReq(new ReqNewLocId(url));
}

void GSNewLocation::OnNewIdResponse(const std::string& id)
{
  if (id.empty())
  {
    SetError("Failed to get new unique ID.");
    return;
  }

std::cout << "Got new location ID from server!! -> " << id << "\n";
  m_locId = id;

  // Directory for this location. Every location has a unique dir to avoid overwriting files.
  std::string dir = "Loc_" + m_locId;

  MkDir(File::GetRoot() + "/" + dir);

  // Need unique location ID for asset file name and data file name
  m_assetFilename = "assets_loc_" + id + ".txt";
  m_dataFilename = "data_loc_" + id + ".txt";  

  // Upload files to server
  m_uploadedFiles = 0;
  m_totalFiles = m_strs.size() + 2;  // plus one for the asset file, one for data file

  // Make an asset file for the Location object, just as a string in mem.
  std::string assetFile = "// Asset file for location " + id + "\r\n// Version\r\n1\r\n";
  
  SetError("Uploading " + ToString(m_totalFiles) + " files, please wait...");

  for (StrSet::iterator it = m_strs.begin(); it != m_strs.end(); ++it)
  {
    const std::string& file = *it;

    assetFile += dir + "/" + StripPath(file) + "\r\n";

    // TODO Check if file already exists - don't upload if it's the same.
    // DO warn about name clashes, showing files which will not be uploaded and files which will be changed.
std::cout << "UPLOAD THIS FILE: " << file << "\n";

    // TODO Hide from this layer the impl detail that we are sending an HTTP request!!!
    // Just send a message to the server.

    std::string url = TheVe1ReqManager::Instance()->MakeUrl(FILE_UPLOAD);
    url += "&dir=" + dir;
    url += "&filename=" + StripPath(file) + "&data=";

    if (FileContentToUrl(file, &url))
    {
      UploadReq* up = new UploadReq(url, StripPath(file));
      // Number of concurrent reqs is number of files
      TheVe1ReqManager::Instance()->AddReq(up, m_totalFiles);
    }
    else
    {
      SetError("Failed to make upload request for file " + file);
      return;
    }
  }

  // Upload asset file
  std::string url = TheVe1ReqManager::Instance()->MakeUrl(FILE_UPLOAD);
  url += "&dir=" + dir;
  url += "&filename=" + m_assetFilename + "&data=" + assetFile;
  url = ToUrlFormat(url);
  TheVe1ReqManager::Instance()->AddReq(new UploadReq(url, m_assetFilename), m_totalFiles);

  // Upload location data file
  url = TheVe1ReqManager::Instance()->MakeUrl(FILE_UPLOAD);
  url += "&dir=" + dir;
  url += "&filename=" + m_dataFilename + "&data=" +  "// version\r\n1\r\n// obj file\r\n" + dir + "/" + m_objFile;
  url = ToUrlFormat(url);
  TheVe1ReqManager::Instance()->AddReq(new UploadReq(url, m_dataFilename), m_totalFiles);

  // Now we wait -- but make sure Online Req Manager is being updated!
}

void GSNewLocation::OnUploadFinished(const UploadInfo& ui)
{
  if (ui.m_ok)
  {
    // No mutex required, responses are handled in main thread
    m_uploadedFiles++;

std::cout << "SUCCESS for " << ui.m_filename << "\n";
  }
  else
  {
    SetError("Failed on " + ui.m_filename);
std::cout << "FAIL for " << ui.m_filename << "\n";
    return;
  }

std::cout << " ...num files uploaded now: " << m_uploadedFiles << "\n";

  if (m_uploadedFiles == m_totalFiles)
  {
    SetError("Finished uploading, creating new location on server...");
  
    std::string dir = "Loc_" + m_locId; // TODO comon func

    // Send req to make new Location game object.
    std::string url = TheVe1ReqManager::Instance()->MakeUrl(CREATE_LOCATION);
    url += "&loc_id=" + m_locId + "&asset_file=" + dir + "/" + m_assetFilename + "&data_file=" + dir + "/" + m_dataFilename;
    url = ToUrlFormat(url);

    TheVe1ReqManager::Instance()->AddReq(new CreateNewLocationReq(url), m_totalFiles);
  }
  else 
  {
    SetError("Uploaded " + ToString(m_uploadedFiles) + " of " + ToString(m_totalFiles) + "  files...");
  } 
}

void GSNewLocation::OnLocationCreated()
{
  // Enter the new location.
  int id = ToInt(m_locId);
std::cout << "OnLocationCreated called ok! Now going to this location: " << id << "...\n";
  SetLocalPlayerLocation(id);
}

static void OnCancelButton()
{
  // Go back to prev state
  TheGame::Instance()->SetCurrentState(TheGSEdit::Instance()); 
}

void GSNewLocation::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-newlocation.txt");
  Assert(m_gui);

  m_gui->GetElementByName("ok-button")->SetCommand(OnOKNew);
  m_gui->GetElementByName("cancel-button")->SetCommand(OnCancelButton);
  // TODO "test" and "commit" buttons

  // Get last path, set path/file browser to it
  RestoreLastPath();
}

void GSNewLocation::RestoreLastPath()
{
  // Get path of previous successfully loaded mesh, set file browser to this path
  GameConfigFile* config = TheGameConfigFile::Instance();
  if (config->Exists(LOCATION_PATH_KEY))
  {
    std::string s = config->GetValue(LOCATION_PATH_KEY);
    GuiText* text = dynamic_cast<GuiText*>(m_gui->GetElementByName("obj_file"));
    Assert(text);
    text->SetText(s); 
  }
}

}

