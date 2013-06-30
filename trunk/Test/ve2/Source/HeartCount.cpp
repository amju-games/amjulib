#include <AmjuFirst.h>
#include "HeartCount.h"
#include "LocalPlayer.h"
#include "Ve1OnlineReqManager.h"
#include <AmjuFinal.h>

namespace Amju
{
////static const char* FOOD_KEY = "food";
////
////bool GetFoodCount(int* result)
////{
////  return GetPlayerCount(FOOD_KEY, result);
////}
////
////bool ChangeFoodCount(int delta)
////{
////  return ChangePlayerCount(FOOD_KEY, delta);
////}
////
////
////static const char* STAMINA_KEY = "stamina";
////
////bool GetHeartCount(int* result)
////{
////  return GetPlayerCount(STAMINA_KEY, result);
////}
////bool ChangeHeartCount(int delta)
////{
////  return ChangePlayerCount(STAMINA_KEY, delta);
////}


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

bool ChangePlayerCount(const std::string& key, int delta)
{
  Player* player = GetLocalPlayer();
  if (!player)
  {
    return false;
  }
  int id = GetLocalPlayerId();
  return ChangeObjCount(id, key, delta);
}

bool ChangeObjCount(int objId, const std::string& key, int delta)
{
  // Must be a server side script to avoid lost data! 
  static const int MAX_CHANGE_HEARTS = 100;

  std::string url = TheVe1ReqManager::Instance()->MakeUrl(CHANGE_VALUE);
  url += "&player_obj_id=" + ToString(objId);
  url += "&key=" + key;
  url += "&change=" + ToString(delta);

  TheVe1ReqManager::Instance()->AddReq(new ReqChangeValue(url), MAX_CHANGE_HEARTS);

  return true;
}
}

