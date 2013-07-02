#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <deque>
#include <Shadow.h>
#include <SceneMesh.h>
#include "Ve1ObjectChar.h"
#include "Sprite.h"

namespace Amju
{
class Food;
class Baddie;

class Player : public Ve1ObjectChar
{
public:
  static const char* TYPENAME;

  Player();
  virtual const char* GetTypeName() const { return TYPENAME; }
  virtual void Update();
  virtual bool Load(File*);

  // Set dir of anything carried
  //virtual void SetDir(float degs);

  // Add arrow to scene graph
  virtual void OnLocationEntry();
  virtual void OnLocationExit();
 
  // NB We don't have a LocalPlayer subclass. This is because if a different user logs in, the
  //  types of the players will be wrong. With a flag it's much easier to change who the local
  //  player is.
  bool IsLocalPlayer() const; 

  // Ve1Object overrides
  virtual void SetKeyVal(const std::string& key, const std::string& val);
  virtual void SetMenu(GuiMenu*);

  // Set position of arrow for this player, i.e. where we want to go
  void SetArrowPos(const Vec3f& pos); 
  void SetArrowVis(bool visible);

  bool IsLoggedIn() const;
  void SetLoggedIn(bool loggedIn);

  void SetCarrying(Ve1Object*);
  Ve1Object* GetCarrying();

  void OnCollideFood(Food* f);

  // New 2D customisable avatars
  LayerSprite& GetSprite();

  int GetFoodRecvCount();
  int GetFoodGivenCount();

  // Called on collision, to give food
  void OnCollidePlayer(Player* otherPlayer);
  void OnCollideBaddie(Baddie* baddie);

  void ShootBaddie(Baddie* baddie);

private:
  void EatFood(Food*);

protected:
  RCPtr<SceneMesh> m_arrow; // destination arrow
  RCPtr<SceneNode> m_nameTag; // visible name shown above player (move to base class ?)

  RCPtr<Ve1Object> m_carrying;

  bool m_isLocal;
  bool m_isLoggedIn;
  bool m_isDead; // if so, must go back to start location

//  int m_lastFoodCount;
//  int m_totalFoods; // total num Food cells brought to spaceship - -1 until set from server

  typedef std::deque<RCPtr<Ve1Object> > CarriedFood;
  CarriedFood m_carriedFood;

  int m_maxHealth;
  int m_health;
};

bool GetNameForPlayer(int objId, std::string* r);

}

#endif

