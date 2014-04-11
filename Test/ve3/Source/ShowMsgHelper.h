#pragma once

namespace Amju
{
class GuiElement;

// Helpful code for GUI classes which show messages.
// * Sets the message text in a GuiText with name "orig-msg-text".
// * Adds Mugshot at top left of the sender
void ShowMsgHelper(int msgId, GuiElement* gui);
}

