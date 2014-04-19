#ifndef GS_NETERROR_H_INCLUDED
#define GS_NETERROR_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
class GSNetError : public GSGui
{
  GSNetError();
  friend class Singleton<GSNetError>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);

  void SetError(const std::string& error);

private:
  std::string m_errorStr;
};

typedef Singleton<GSNetError> TheGSNetError;

// Show an error, setting state to return to on retry
void ShowError(const std::string&, GameState* prevState);

// Don't change whatever the previous state has been set to
void ShowError(const std::string&);

} // namespace
#endif
