#ifndef GS_FILEUPDATECHECK_H_INCLUDED
#define GS_FILEUPDATECHECK_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
class GSFileUpdateCheck : public GSGui
{
  GSFileUpdateCheck();
  friend class Singleton<GSFileUpdateCheck>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  void OnSkip(); // just go to next state

  void OnFinishedChecking(const std::string& timestamp); // called when we have got final file from server 

  void NextState();

  void SetNumFilesToWaitFor(int n, const std::string newtimestamp);
  void OnDownloadedFile(const std::string& filename);

private:
  std::string m_timestamp;
  std::string m_newtimestamp;

  int m_numFilesToWaitFor;
  int m_numFilesDownloaded;
};
typedef Singleton<GSFileUpdateCheck> TheGSFileUpdateCheck;
} // namespace
#endif
