#ifndef GUI_FACTORY_H
#define GUI_FACTORY_H

#include "Factory.h"
#include "Singleton.h"
#include "GuiElement.h"

namespace Amju
{
class GuiFactory : public Factory<GuiElement>
{
public:
  // Populate factory with known gui types
  GuiFactory();
};

typedef Singleton<GuiFactory> TheGuiFactory;
}

#endif
