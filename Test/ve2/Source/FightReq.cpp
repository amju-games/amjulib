#include <AmjuFirst.h>
#include "FightReq.h"
#include "Ve1Req.h"
#include "Ve1OnlineReqManager.h"
#include "LocalPlayer.h"
#include <AmjuFinal.h>

namespace Amju
{
class FightReq : public Ve1Req
{
public:
  FightReq(const std::string& url, Ve1ObjectChar* opponent) : Ve1Req(url, "fight"), m_opponent(opponent)
  {
  }

  void OnSuccess()
  {
    // Find out who took damage, display in HUD
    // TODO
std::cout << GetResult().GetString() << "\n";

    m_opponent->ShowAttacked();
    //CheckCollects();
  }

private:
  Ve1ObjectChar* m_opponent;
};

void SendFightReq(Ve1ObjectChar* opponent)
{
  std::string url = TheVe1ReqManager::Instance()->MakeUrl(FIGHT);
  url += "&id=" + ToString(GetLocalPlayerId());
  url += "&opp=" + ToString(opponent->GetId());

  TheVe1ReqManager::Instance()->AddReq(new FightReq(url, opponent));
}

}

