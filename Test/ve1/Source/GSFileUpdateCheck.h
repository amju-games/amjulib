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

  void OnSkip(); // just go to next state..?

  void OnFinishedChecking(const std::string& timestamp); // called when we have got response from server 

private:
  std::string m_timestamp;
};
typedef Singleton<GSFileUpdateCheck> TheGSFileUpdateCheck;
} // namespace
#endif
