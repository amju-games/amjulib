#include <AmjuFirst.h>
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
#include "SaveConfig.h"
#include <GuiFileDialog.h>
#include "Terrain.h"
#include "GSWaitForUpload.h"
#include "ObjectManager.h"
#include "ObjectUpdater.h"
#include "ReqSetObjectFilenames.h"
#include <AmjuFinal.h>

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
    const HttpResult& res = GetResult();

std::cout << "FAILED to get new location ID.\n";
std::cout << "RESULT: " << res.GetString() << "\n";
std::cout << "ERROR: " << res.GetErrorString() << "\n";
std::cout << "Error from Ve1Req: " << m_errorStr << "\n";
std::cout << "URL was: \"" << m_url << "\"\n";

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
  m_mode = AMJU_ADD_NEW;
}

std::string GSNewLocation::MakeLocDir() const
{
  return "Loc_" + m_locId;
}

void GSNewLocation::SetMode(GSNewLocation::Mode mode)
{
  m_mode = mode;
}

void GSNewLocation::SetError(const std::string& str)
{
  if (m_gui)
  {
    GuiText* text = dynamic_cast<GuiText*>(m_gui->GetElementByName("error"));
    Assert(text);
    text->SetText(str);
  }
}

void GSNewLocation::OnOKButton()
{
  // Save path/file in prefs.

  // Try loading .obj file. Temporarily set File Root to path to .obj and other files.
  ////GuiText* text = dynamic_cast<GuiText*>(m_gui->GetElementByName("obj_file"));
  GuiText* text = dynamic_cast<GuiText*>(m_gui->GetElementByName("fd-path-text"));
  Assert(text);
  std::string pathFile = text->GetText();

  // Remember this path
  static GameConfigFile* config = TheGameConfigFile::Instance();
  config->Set(LOCATION_PATH_KEY, pathFile);
  SaveConfig();


std::cout << "Attempting to load Location obj: " << pathFile << "\n";

  std::string path = GetFilePath(pathFile);
  std::string origRoot = File::GetRoot();
  File::SetRoot(path , "/"); // TODO change SetRoot params

  m_objFile = StripPath(pathFile);

  // TODO Bypass Res Manager so we don't cache the file, we want to reload it.
  // Also, no path is given here so files with the same name in different dirs
  //  won't work!!!!
  bool binary = (GetFileExt(m_objFile) == "objb");
  RCPtr<ObjMesh> mesh = new ObjMesh;
  bool loaded = mesh->Load(m_objFile, binary); 

  if (loaded)
  {
    std::cout << "Loaded obj file ok!!\n";
    /*
    if (!binary)
    {
      mesh->Save(m_objFile, false); // Save optimised files 
    }
    */
  }
  else
  {
    SetError("Failed to load obj file " + m_objFile);
    return;
  }

  // After loading, revert to original file root
  File::SetRoot(origRoot, "/");

  // TODO Create path + filename
  //if (GetTerrain())
  //{
  //  GetTerrain()->SetMeshFilename(MakeLocDir() + "/" + m_objFile);
  //}
  //else
  //{
  //  std::cout << "No terrain in this location!?!\n";
  //}

  // When we re-enter location, we should load the new file.
  // Make sure file is not cached by ResourceManager
  TheResourceManager::Instance()->Clear(); 

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
  if (m_mode == AMJU_EDIT)
  {
    // No need to request a new ID. Use the current ID and go straight to the next bit.
    OnNewIdResponse(m_locId);
  }
  else
  {
    // Request a new location ID. Use this for asset file and data file.
    std::string url = TheVe1ReqManager::Instance()->MakeUrl(GET_NEW_LOCATION_ID);
    url += "&name=nameTODO";
    TheVe1ReqManager::Instance()->AddReq(new ReqNewLocId(url));
  }
}

void OnFinishedUpload()
{
  TheGSNewLocation::Instance()->OnFinishedUpload();
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
  std::string dir = MakeLocDir(); //"Loc_" + m_locId;

  // No good for other clients who didn't create this location!
  //MkDir(File::GetRoot() + "/" + dir);

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
//  url = ToUrlFormat(url);
  TheVe1ReqManager::Instance()->AddReq(new UploadReq(url, m_assetFilename), m_totalFiles);

  // Upload location data file
  url = TheVe1ReqManager::Instance()->MakeUrl(FILE_UPLOAD);
  url += "&dir=" + dir;
  url += "&filename=" + m_dataFilename + "&data=" +  "// version\r\n1\r\n// obj file\r\n" + dir + "/" + m_objFile;
//  url = ToUrlFormat(url);
  TheVe1ReqManager::Instance()->AddReq(new UploadReq(url, m_dataFilename), m_totalFiles);

  // Now we wait -- but make sure Online Req Manager is being updated!
  static GSWaitForUpload* wfu = TheGSWaitForUpload::Instance();
  wfu->SetOnFinishedFunc(Amju::OnFinishedUpload);
  wfu->SetTotalFiles(m_totalFiles);
  TheGame::Instance()->SetCurrentState(wfu);
}

void GSNewLocation::OnFinishedUpload()
{
  std::string dir = MakeLocDir();

  if (m_mode == AMJU_EDIT)
  {
std::cout << "All uploaded, we are done here!\n";

    // Trash map of downloaded files
    TheObjectManager::Instance()->TrashMap(); // shouldn't be necessary

    // Touch object
//    SendReqSetObjectFilenames(GetTerrain()->GetId(), dir + "/" + m_assetFilename, dir + "/" + m_dataFilename);

    // Delete current terrain so we are forced to re-download
    TheObjectManager::Instance()->Clear(); 
    TheObjectUpdater::Instance()->Clear(); 

//    ClearTerrain(); // shouldn't be necessary

    OnLocationCreated();
  }
  else
  {
    SetError("Finished uploading, creating new location on server...");
 
    // Send req to make new Location game object.
    std::string url = TheVe1ReqManager::Instance()->MakeUrl(CREATE_LOCATION);
    url += "&loc_id=" + m_locId + "&asset_file=" + dir + "/" + m_assetFilename + "&data_file=" + dir + "/" + m_dataFilename;
//    url = ToUrlFormat(url);
    TheVe1ReqManager::Instance()->AddReq(new CreateNewLocationReq(url), m_totalFiles);
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

  if (m_mode == AMJU_EDIT)
  {
    m_locId = ToString(GetLocalPlayerLocation());
  }

  m_gui = LoadGui("gui-newlocation.txt");
  Assert(m_gui);

  GetElementByName(m_gui, "ok-button")->SetCommand(OnOKNew);
  GetElementByName(m_gui, "cancel-button")->SetCommand(OnCancelButton);
  // Text edit control needs focus to accept kb input
  //GetElementByName(m_gui, "obj_file")->SetHasFocus(true);

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

    GuiFileDialog* dlg = (GuiFileDialog*)(m_gui->GetElementByName("my-file-dialog"));
    Assert(dlg);
    dlg->SetPathAndFile(s);
  }
}

}

