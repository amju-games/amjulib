#include "FileUpdater.h"
#include <iostream>
#include <SafeUtils.h>
#include <Directory.h>
#include <Game.h>
#include "Timestamp.h"
#include "MsgManager.h"
#include "LocalPlayer.h"
#include <StringUtils.h>
#include "GSNetError.h"
#include "GSFileUpdateCheck.h"

#define XML_DEBUG

namespace Amju
{
FileUpdater::FileUpdater(const std::string& url) : Ve1Req(url, "file-update")
{
}

void FileUpdater::OnSuccess()
{
  // Do nothing if GSFileUpdateCheck is no longer the current state - this means we chose
  //  to skip this check.
  // TODO
  if (TheGame::Instance()->GetState() != TheGSFileUpdateCheck::Instance())
  {
std::cout << "SKIPPED FILE UPDATE CHECK, SO IGNORING SERVER RESPONSE!!\n";
    return;
  }

  PXml p = m_xml.getChildNode(1);
  if (SafeStrCmp(p.getName(), "files"))
  {
#ifdef XML_DEBUG
std::cout << "Found files element\n";
#endif

    // Get children - each child is of form <file>$filename</file>. Delete each file(!)
    int num = p.nChildNode();
std::cout << num << "children\n";
    for (int i = 0; i < num; i++)
    {
      PXml child = p.getChildNode(i);
      if (!SafeStrCmp(child.getName(), "file"))
      {
std::cout << "Got 'files' but no 'file' child\n";
        OnFailure();
      }
      std::string f = child.getText();
      std::string full = File::GetRoot() + f;

std::cout << "DELETE THIS FILE: \"" << full << "\"\n";
      if (AmjuDeleteFile(full))
      {
std::cout << " -- succeeded!\n";
      }
      else
      {
std::cout << " -- failed!\n"; 
      }
      // TODO and tell ObjectManager/ObjectUpdater to remove info about this file from their caches!

    } 
    // Done - now prod ObjectManager to get new files
std::cout << "Have finished deleting updated files, now need to download new versions!\n";

    TheGSFileUpdateCheck::Instance()->OnFinishedChecking(m_timestamp);
  }
  else
  {
    OnFailure();
  }

}

void FileUpdater::OnFailure()
{
std::cout << "FAILED to get updated file list from server.\n";
  const HttpResult& res = GetResult();

std::cout << "RESULT: " << res.GetString() << "\n";
std::cout << "ERROR: " << res.GetErrorString() << "\n";
std::cout << "Error from Ve1Req: " << m_errorStr << "\n";
std::cout << "URL was: \"" << m_url << "\"\n";

  TheGSFileUpdateCheck::Instance()->OnFinishedChecking(m_timestamp); // ??
}

}


