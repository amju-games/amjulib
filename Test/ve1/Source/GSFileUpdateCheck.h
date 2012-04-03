#ifndef GS_FILEUPDATECHECK_H_INCLUDED
#define GS_FILEUPDATECHECK_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
class GSFileUpdateCheck : public GSGui
{
  GSFileUpdateCheck();
  friend class Singleton<GSFileUpdateCheck>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);

  void OnSkip(); // just go to next state..?

  void OnFinishedChecking(); // called when we have got response from server 

  void SetNextState(GameState* next);

private:
  std::string m_timestamp;
  GameState* m_next;
};
typedef Singleton<GSFileUpdateCheck> TheGSFileUpdateCheck;
} // namespace
#endif
