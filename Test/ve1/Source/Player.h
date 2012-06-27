#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "Ve1ObjectChar.h"
#include <Shadow.h>
#include <SceneMesh.h>

namespace Amju
{
class Player : public Ve1ObjectChar
{
public:
  static const char* TYPENAME;

  Player();
  virtual const char* GetTypeName() const { return TYPENAME; }
  virtual void Update();
  virtual bool Load(File*);

  virtual void OnLocationEntry();
  virtual void OnLocationExit();
 
  bool Init(); // TODO why ?

  // NB We don't have a LocalPlayer subclass. This is because if a different user logs in, the
  //  types of the players will be wrong. With a flag it's much easier to change who the local
  //  player is.
  bool IsLocalPlayer() const; 
//  void SetIsLocalPlayer(bool);

  // Ve1Object overrides
  virtual void SetKeyVal(const std::string& key, const std::string& val);
  virtual void SetMenu(GuiMenu*);

  // Set position of arrow for this player, i.e. where we want to go
  void SetArrowPos(const Vec3f& pos); 
  void SetArrowVis(bool visible);

  bool IsLoggedIn() const;

protected:
  bool m_isLocal;
  bool m_isLoggedIn;

  RCPtr<SceneMesh> m_arrow; // destination arrow
  RCPtr<SceneNode> m_nameTag; // visible name shown above player (move to base class ?)
};

bool GetNameForPlayer(int objId, std::string* r);

}

#endif

