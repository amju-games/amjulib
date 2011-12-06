#ifndef DOWNLOAD_MANAGER_H
#define DOWNLOAD_MANAGER_H

#include <map>
#include <string>

namespace Amju
{
class DownloadManager
{
public:
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

private:

  typedef std::map<std::string, State> FileMap;
  FileMap m_map;
};
}

#endif