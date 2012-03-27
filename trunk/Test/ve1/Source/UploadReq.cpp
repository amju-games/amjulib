#include "UploadReq.h"
#include "GSNewLocation.h"

namespace Amju
{
UploadReq::UploadReq(const std::string& url, const std::string& filename) : Ve1Req(url, "upload", HttpClient::POST), m_filename(filename)
{
}

void UploadReq::OnSuccess()
{
std::cout << "Upload SUCCESS for " << m_filename << "!\n";

  TheGSNewLocation::Instance()->OnUploadFinished(UploadInfo(true, m_filename));
}

void UploadReq::OnFailure()
{
std::cout << "Upload FAILED for " << m_filename << "!\n";
  const HttpResult& res = GetResult();

std::cout << "RESULT: " << res.GetString() << "\n";

std::cout << "ERROR: " << res.GetErrorString() << "\n";

std::cout << "Error from Ve1Req: " << m_errorStr << "\n";
//std::cout << "URL: " << m_url << "\n";
  
  TheGSNewLocation::Instance()->OnUploadFinished(UploadInfo(false, m_filename));
}

}


