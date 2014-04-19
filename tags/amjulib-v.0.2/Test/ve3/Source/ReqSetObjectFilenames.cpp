#include <AmjuFirst.h>
#include <Game.h>
#include <StringUtils.h>
#include <UrlUtils.h>
#include "ReqSetObjectFilenames.h"
#include "Ve1OnlineReqManager.h"
#include "GSEdit.h"
#include "GSObjMesh.h"
#include <AmjuFinal.h>

namespace Amju
{
void SendReqSetObjectFilenames(int objId, const std::string& assetFilename, const std::string& dataFilename)
{
  // Send req to make new game object.
  std::string id = ToString(objId);

  std::string url = TheVe1ReqManager::Instance()->MakeUrl(CREATE_OBJECT);
  url += "&obj_id=" + id + "&asset_file=" + assetFilename + "&data_file=" + dataFilename;
//  url = ToUrlFormat(url);
  TheVe1ReqManager::Instance()->AddReq(new ReqSetObjectFilenames(url), 1);
  // Only change one object's files at a time, right ?
}

ReqSetObjectFilenames::ReqSetObjectFilenames(const std::string& url) : Ve1Req(url, "create object") 
{
}

void ReqSetObjectFilenames::OnSuccess()
{
std::cout << "Create new object succeeded!\n";
  // Go back to Edit state: the new object should be magically updated via the regular
  //  ObjectUpdater mechanism, right ?
  TheGame::Instance()->SetCurrentState(TheGSEdit::Instance());
}

void ReqSetObjectFilenames::OnFailure()
{
std::cout << "Create new object FAILED!\n";

  const HttpResult& res = GetResult();

std::cout << "RESULT: " << res.GetString() << "\n";
std::cout << "ERROR: " << res.GetErrorString() << "\n";
std::cout << "Error from Ve1Req: " << m_errorStr << "\n";

  TheGSObjMesh::Instance()->SetError("Failed to create new object.");
  // TODO Error msg
}

}


