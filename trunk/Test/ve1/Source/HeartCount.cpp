#include "HeartCount.h"
#include "LocalPlayer.h"
#include "Ve1OnlineReqManager.h"

namespace Amju
{
bool GetHeartCount(int* result)
{
  Player* p = GetLocalPlayer();
  if (p)
  {
    static const char* STAMINA_KEY = "stamina";
    if (p->Exists(STAMINA_KEY))
    {
      std::string s = p->GetVal(STAMINA_KEY);
      *result = ToInt(s);
      return true;
    }
  }
  return false;
}

class ReqChangeValue : public Ve1Req
{
public:
  ReqChangeValue(const std::string& url) : Ve1Req(url, "changevalue", HttpClient::GET)
  {
  }

  void OnSuccess()
  {
    std::cout << "Successfully sent change!\n";
  }
};

bool ChangeHeartCount(int delta)
{
  Player* player = GetLocalPlayer();
  if (!player)
  {
    return false;
  }

  // Must be a server side script to avoid lost data! 
  static const int MAX_CHANGE_HEARTS = 100;
  static const std::string STAMINA_KEY = "stamina";

  std::string url = TheVe1ReqManager::Instance()->MakeUrl(CHANGE_VALUE);
  url += "&player_obj_id=" + player->GetId();
  url += "&key=" + STAMINA_KEY;
  url += "&change=" + ToString(delta);

  TheVe1ReqManager::Instance()->AddReq(new ReqChangeValue(url), MAX_CHANGE_HEARTS);

  return true;
}
}

