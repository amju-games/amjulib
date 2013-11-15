#ifndef GS_TITLE_H_INCLUDED
#define GS_TITLE_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"
#include "TitlePerson.h"

namespace Amju 
{
class GSTitle : public GSGui
{
  GSTitle();
  friend class Singleton<GSTitle>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();
  virtual void OnDeactive();

private:
  GuiImage m_titleImage;
  TitlePeople m_tp;
};
typedef Singleton<GSTitle> TheGSTitle;
} // namespace
#endif
