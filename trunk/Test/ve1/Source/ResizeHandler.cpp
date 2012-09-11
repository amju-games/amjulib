#include <iostream>

#include "ResizeHandler.h"

namespace Amju
{
bool ResizeHandler::OnResizeEvent(const ResizeEvent& re)
{
std::cout << "RESIZE EVENT! Save this shit to game.cfg!!!\n";
}
}

