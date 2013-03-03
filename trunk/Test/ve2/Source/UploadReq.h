#ifndef UPLOAD_REQ_H_INCLUDED
#define UPLOAD_REQ_H_INCLUDED

#include "Ve1Req.h"

namespace Amju
{
class UploadReq : public Ve1Req
{
public:
  UploadReq(const std::string& url, const std::string& filename);
  virtual void OnSuccess();
  virtual void OnFailure();

private:
  std::string m_filename;
};

struct UploadInfo
{
  UploadInfo(bool ok, const std::string& filename) : m_ok(ok), m_filename(filename) {}

  bool m_ok;
  std::string m_filename;
};
}

#endif

