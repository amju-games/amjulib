#ifndef GS_WAITFORUPLOAD_H_INCLUDED
#define GS_WAITFORUPLOAD_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"
#include "UploadReq.h"

namespace Amju 
{
class GSWaitForUpload : public GSGui
{
  GSWaitForUpload();
  friend class Singleton<GSWaitForUpload>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  // Called when an UploadReq finishes
  void OnUploadFinished(const UploadInfo& ui);

  void SetError(const std::string& str);

  typedef void (*Func)();
  void SetOnFinishedFunc(Func f) { m_onFinishedFunc = f; }
  void SetTotalFiles(int t) { m_totalFiles = t; m_uploadedFiles = 0; }

private:
  // Callback: we call this on successful upload. Set before activating this function
  Func m_onFinishedFunc;
 
  // We upload files to the server - track total and how many so far
  int m_uploadedFiles;
  int m_totalFiles;
};
typedef Singleton<GSWaitForUpload> TheGSWaitForUpload;
} // namespace
#endif
