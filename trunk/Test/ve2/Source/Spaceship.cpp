#include <AmjuFirst.h>
#include <GameObjectFactory.h>
#include "Spaceship.h"
#include "HeartCount.h"
#include "GSMain.h"
#include "ResearchCalendar.h"
#include "LurkMsg.h"
#include "GameConsts.h"
#include <AmjuFinal.h>

namespace Amju
{
const char* Spaceship::TYPENAME = "spaceship";

static const std::string& GetKey()
{
  static const std::string k = "fuelcells_day" + ToString(TheResearchCalendar::Instance()->GetDayOnPlanet());
  return k;
}

GameObject* CreateSpaceship()
{
  return new Spaceship;
}

static bool registered = TheGameObjectFactory::Instance()->Add(Spaceship::TYPENAME, CreateSpaceship);

const char* Spaceship::GetTypeName() const
{
  return TYPENAME;
}

Spaceship::Spaceship()
{
}

void Spaceship::SetKeyVal(const std::string& key, const std::string& val)
{
  Building::SetKeyVal(key, val);

  if (key == GetKey())
  {
    // TODO Set count member var too ?
    // TODO AChievements - e.g. first one, 5th, 10th, 20th, etc.
    int fc = ToInt(val);

    std::string str;
    if (fc == 1 || fc == 0)
    {
      str = "The spaceship now has one fuel cell!";
    }
    else
    {
      str = "The spaceship now has a total of " + val + " fuel cells!";
    }
    TheMsgManager::Instance()->QueueMsg(MsgManager::Msg(0, str, -2, -2, Time::Now())); 
    // Lurk msg "annoying"
    //LurkMsg lm(str, LURK_FG, LURK_BG, AMJU_CENTRE);
    //TheLurker::Instance()->Queue(lm);

    // Check for meeting objective for the day
  }
}

void Spaceship::AddFuel(int delta)
{
  // Player collided with us, giving us this many fuel cells.

  if (delta != 0)
  {
    // Count goes up for spaceship grand total
    ChangeObjCount(GetId(), GetKey(), delta);
  }
}

} // namespace

