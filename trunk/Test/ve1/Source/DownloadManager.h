#ifndef DOWNLOAD_MANAGER_H
#define DOWNLOAD_MANAGER_H

#include <map>
#include <string>

namespace Amju
{
class DownloadManager
{
public:
  // If we have too many requests, GetFile() will fail, and you should retry.
  static const int MAX_CONCURRENT_DOWNLOADS = 16;  

  bool GetFile(const std::string& filename);

  bool IsLocal(const std::string& filename);

  enum State 
  { 
    AMJU_DL_UNKNOWN, 
    AMJU_DL_LOCAL, 
    AMJU_DL_DOWNLOADING, 
    AMJU_DL_FAILED 
  };
  void SetState(const std::string& filename, State);

  // Empty map (used in Edit mode to make sure we download changed files)
  void TrashMap();

private:

  typedef std::map<std::string, State> FileMap;
  FileMap m_map;
};
}

#endif