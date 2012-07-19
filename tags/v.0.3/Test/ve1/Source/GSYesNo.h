#ifndef GS_YESNO_H_INCLUDED
#define GS_YESNO_H_INCLUDED

#include "GSGui.h"

namespace Amju 
{
class GSYesNo : public GSGui
{
public:
  GSYesNo();
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  virtual void OnYes() = 0;
  virtual void OnNo() { GoBack(); } 

protected:
  std::string m_question;
  std::string m_yesText;
  std::string m_noText;
};
} // namespace
#endif
