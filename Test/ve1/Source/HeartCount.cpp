#include "HeartCount.h"
#include "LocalPlayer.h"
#include "Ve1OnlineReqManager.h"

namespace Amju
{
bool GetHeartCount(int* result)
{
  static const char* STAMINA_KEY = "stamina";
  return GetPlayerCount(STAMINA_KEY, result);
}

bool GetFuelCellCount(int* result)
{
  static const char* FUELCELL_KEY = "fuelcells";
  return GetPlayerCount(FUELCELL_KEY, result);
}

bool GetPlayerCount(const std::string& key, int* result)
{
  Player* p = GetLocalPlayer();
  if (p)
  {
    if (p->Exists(key))
    {
      std::string s = p->GetVal(key);
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
  static const std::string STAMINA_KEY = "stamina";
  return ChangePlayerCount(STAMINA_KEY, delta);
}

bool ChangePlayerCount(const std::string& key, int delta)
{
  Player* player = GetLocalPlayer();
  if (!player)
  {
    return false;
  }

  // Must be a server side script to avoid lost data! 
  static const int MAX_CHANGE_HEARTS = 100;

  std::string url = TheVe1ReqManager::Instance()->MakeUrl(CHANGE_VALUE);
  url += "&player_obj_id=" + ToString(player->GetId());
  url += "&key=" + key;
  url += "&change=" + ToString(delta);

  TheVe1ReqManager::Instance()->AddReq(new ReqChangeValue(url), MAX_CHANGE_HEARTS);

  return true;
}
}

