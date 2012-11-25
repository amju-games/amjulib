#ifndef GS_AVATARMOD_H_INCLUDED
#define GS_AVATARMOD_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"
#include "Ve1Character.h"

namespace Amju 
{
class GSAvatarMod : public GSGui
{
  GSAvatarMod();
  friend class Singleton<GSAvatarMod>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  void OnNextType();
  void OnPrevType();
  void OnCancel();
  void OnOk();

private:
  void CreateChar();
  void CheckAvailable();

private:
  //typedef std::vector<RCPtr<Ve1Character> > Characters;
  //Characters m_chars;

  RCPtr<Ve1Character> m_char;

  std::string m_currentChar;
  int m_current;
  int m_howManyCharsAvailable; // increases with Achievements
};
typedef Singleton<GSAvatarMod> TheGSAvatarMod;
} // namespace
#endif
