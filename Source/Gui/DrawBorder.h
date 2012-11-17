#ifndef DRAW_BORDER_H_INCLUDED
#define DRAW_BORDER_H_INCLUDED

namespace Amju
{
class GuiElement;
struct Colour;

// Offset is extra distance around the bounding rectangle of the 
//  GUI element
void DrawBorder(GuiElement* gui, const Colour& col, float offset = 0);

void DrawFilled(GuiElement* gui, const Colour& col, float offset = 0);
}

#endif