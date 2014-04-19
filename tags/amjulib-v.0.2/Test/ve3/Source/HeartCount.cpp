#include <AmjuFirst.h>
#include "HeartCount.h"
#include "LocalPlayer.h"
#include "Ve1OnlineReqManager.h"
#include "ObjectManager.h"
#include "Ve1Object.h"
#include <AmjuFinal.h>

namespace Amju
{
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
    std::cout << "Successfully sent change! " + m_url + "\n";
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
  if (delta == 0)
  {
std::cout << "Discarding zero change for key: " << key << " obj: " << objId << "\n";
    return true;
  }

  // Must be a server side script to avoid lost data! 
  static const int MAX_CHANGE_HEARTS = 100;

  std::string url = TheVe1ReqManager::Instance()->MakeUrl(CHANGE_VALUE);
  url += "&player_obj_id=" + ToString(objId);
  url += "&key=" + key;
  url += "&change=" + ToString(delta);

  TheVe1ReqManager::Instance()->AddReq(new ReqChangeValue(url), MAX_CHANGE_HEARTS);

  // Immediately change object on this local client
  GameObject* go = TheObjectManager::Instance()->GetGameObject(objId);
  Ve1Object* ww = dynamic_cast<Ve1Object*>(go);
  Assert(ww);
  int newVal = delta;
  if (ww->Exists(key))
  {
    newVal = ToInt(ww->GetVal(key)) + delta;
  }
  ww->SetKeyVal(key, ToString(newVal));
  return true;
}
}

