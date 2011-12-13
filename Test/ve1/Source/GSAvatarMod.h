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

  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);

  bool LoadCharList();

  void OnNextType();
  void OnPrevType();
  void OnCancel();
  void OnOk();

private:
  typedef std::vector<RCPtr<Ve1Character> > Characters;
  Characters m_chars;

  int m_currentChar;
};
typedef Singleton<GSAvatarMod> TheGSAvatarMod;
} // namespace
#endif
