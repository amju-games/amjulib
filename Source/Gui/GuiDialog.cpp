#include <AmjuFirst.h>
#include <DrawRect.h>
#include <AmjuGL.h>
#include <LoadVec2.h>
#include "GuiDialog.h"
#include <AmjuFinal.h>

namespace Amju
{
const char* GuiDialog::NAME = "gui-dialog";

bool GuiDialog::Load(File* f)
{
  // Read in name, then filename of layout file. Load GUI widgets from this file.
  if (!f->GetDataLine(&m_name))
  {
    f->ReportError("Expected name for dialog");
    return false;
  }

// OK, this must have seemed like a good idea at the time but I can't
//  now remember WHY a dialog is split into 2 files. WHY??!!
#ifdef I_EVER_REMEMBER_WHY

  std::string filename;
  if (!f->GetDataLine(&filename))
  {
    f->ReportError("Expected layout file name for dialog");
    return false;
  }

  File f2;
  if (!f2.OpenRead(filename))
  {
    return false;
  }

#else

  // No filename for the second file, just continue on in the same file.
  File& f2 = *f;

#endif 

  // Like GuiWindow::Load, but we don't want to load in the name again. This would be
  //  only a generic name in the template file, not a unique name.
  //if (!GuiWindow::Load(&f2))
  //{
  //  return false;
  //}
  if (!LoadVec2(&f2, &m_localpos))
  {
    Assert(0);
    return false;
  }

  if (!LoadVec2(&f2, &m_size))
  {
    Assert(0);
    return false;
  }

  return LoadChildren(&f2);
}

void GuiDialog::Draw()
{
  // TODO Tiled background
  // Draw bg - TODO colour, texture ? Rounded corners etc ?
  Rect r = GetRect(this);
  AmjuGL::PushAttrib(AmjuGL::AMJU_TEXTURE_2D);
  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
  AmjuGL::SetColour(Colour(0.5f, 0.5f, 0.5f, 1));
  DrawSolidRect(r);
  AmjuGL::PopAttrib(); 
  

  GuiWindow::Draw();
}
}

