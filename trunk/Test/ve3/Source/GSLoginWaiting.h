#ifndef GS_LOGINWAITING_H_INCLUDED
#define GS_LOGINWAITING_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
class GSLoginWaiting : public GSGui
{
  GSLoginWaiting();
  friend class Singleton<GSLoginWaiting>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  void SetErrorString(const std::string& error);
  void SetEmail(const std::string& email) { m_email = email; }

private:
  std::string m_email; // used to create login request
};
typedef Singleton<GSLoginWaiting> TheGSLoginWaiting;
} // namespace
#endif
