#include <AmjuGL.h>
#include <Game.h>
#include <ConfigFile.h>
#include <GuiTextEdit.h>
#include <UrlUtils.h>
#include "GSObjMesh.h"
#include "GSEdit.h"
#include "SaveConfig.h"
#include "UploadNewContent.h"
#include "GSWaitForUpload.h"
#include "ReqSetObjectFilenames.h"

namespace Amju
{
static const char* MESH_PATH_KEY = "last_mesh_path";

static void OnOKButton()
{
  TheGSObjMesh::Instance()->OnOKButton();
}

static void OnCancelButton()
{
  TheGame::Instance()->SetCurrentState(TheGSEdit::Instance());
}

static void OnFinishedUpload()
{
  TheGSObjMesh::Instance()->OnFinishedUpload();
}


GSObjMesh::GSObjMesh()
{
  m_objId = 0;
  m_uploadedFiles = 0;
  m_totalFiles = 0;
  // TODO MODE
}

void GSObjMesh::SetId(int iid)
{
  m_objId = iid;
  std::string id = ToString(m_objId); 
  m_assetFilename = "assets_obj_" + id + ".txt";
  m_dataFilename = "data_obj_" + id + ".txt";
  m_assetFileContents = 
    "// Asset file for obj " + id + ", ver\r\n1\r\n";
  m_dataFileContents = 
    "// Data file for obj " + id + ", ver\r\n1\r\n";
}

void GSObjMesh::SetError(const std::string& str)
{
  if (!m_gui)
  {
std::cout << "ERROR: Attempt to set GSObjMesh error message, but is not active. Race condition!?!\n";
std::cout << "The msg was:" << str << "\n";
    return;
  }

  GuiText* text = dynamic_cast<GuiText*>(m_gui->GetElementByName("error"));
  Assert(text);
  text->SetText(str);
}

void GSObjMesh::OnFinishedUpload()
{
  // Make sure the GUI is loaded ?? Race condition ??
  TheGame::Instance()->SetCurrentState(this);

  // Now send DB query to update row in Object table - we must set asset file and data file.
  // Can't SetError here, we are not active yet!!

std::cout << "Finished uploading, creating new object on server...\n";

  SendReqSetObjectFilenames(m_objId, m_assetFilename, m_dataFilename);

//  // Send req to make new game object.
//  std::string id = ToString(m_objId); 
//  std::string url = TheVe1ReqManager::Instance()->MakeUrl(CREATE_OBJECT);
//  url += "&obj_id=" + id + "&asset_file=" + m_assetFilename + "&data_file=" + m_dataFilename;
//  url = ToUrlFormat(url);
//  TheVe1ReqManager::Instance()->AddReq(new CreateNewObjectReq(url), m_totalFiles);
}

// Naw, lots of objects could use the same mesh.
//std::string GSObjMesh::MakeDir() const
//{
//  return "Obj_" + m_objId;
//}

void GSObjMesh::OnOKButton()
{
  // Get path + file name
  GuiText* text = dynamic_cast<GuiText*>(m_gui->GetElementByName("obj_file"));
  Assert(text);
  std::string pathFile = text->GetText();

  // Remember this path
  static GameConfigFile* config = TheGameConfigFile::Instance();
  config->Set(MESH_PATH_KEY, pathFile);
  SaveConfig();

  if (CheckCanLoadObj(pathFile))
  {
    SetError("Loaded obj file ok!");    

    std::string id = ToString(m_objId);

    bool ok = UploadObjFile(
      pathFile,
      "", // Server dir - check empty works ok
      m_assetFilename,
      m_assetFileContents, 
      m_dataFilename,
      m_dataFileContents +  "obj mesh\r\n" + StripPath(pathFile) + "\r\n",
      &m_totalFiles);

    if (ok)
    {
      static GSWaitForUpload* wfu = TheGSWaitForUpload::Instance();
      wfu->SetOnFinishedFunc(Amju::OnFinishedUpload);
      wfu->SetTotalFiles(m_totalFiles);
      TheGame::Instance()->SetCurrentState(wfu);
    }
    else
    {
      SetError("Uploading FAILED!");
    } 
  }
  else
  {
    SetError("Failed to load obj file. Is filename correct ?");    
  }
}

void GSObjMesh::Update()
{
  GSGui::Update();

}

void GSObjMesh::Draw()
{
  GSGui::Draw();

}

void GSObjMesh::Draw2d()
{
  GSGui::Draw2d();
}

void GSObjMesh::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-newlocation.txt");
  Assert(m_gui);

  GetElementByName(m_gui, "ok-button")->SetCommand(Amju::OnOKButton);
  GetElementByName(m_gui, "cancel-button")->SetCommand(Amju::OnCancelButton);

  // Text edit control needs focus to accept kb input ??
  GuiTextEdit* edit = (GuiTextEdit*)GetElementByName(m_gui, "obj_file");
  edit->SetHasFocus(true);
  static GameConfigFile* config = TheGameConfigFile::Instance();
  if (config->Exists(MESH_PATH_KEY))
  {
    std::string pathFile = config->GetValue(MESH_PATH_KEY);
    edit->SetText(pathFile);
  } 
}

} // namespace
