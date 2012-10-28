#ifndef FIRST_TIME_MSG_H_INCLUDED
#define FIRST_TIME_MSG_H_INCLUDED

#include <string>

namespace Amju
{
typedef int MsgNum;

// Show a message - but only the first time the player triggers it. 
// Once shown a flag is set on server, so the message is not shown again to that player.
//
// This is intended as a convenient way to add a tutorial message from within the code.
// This should NOT be the only way to add tutorial messages though! Most messages should
// be data driven. 
// NB The msg text will be looked up (for a localised version of the string) in this function.
//
// Can show message in-game (lurk msg) or switch to "story mode".
//
// Return true if message is displayed, else false if already shown previously.
//
// TODO Make a message struct with voice over. Multiple page message would be a vector etc.
// TODO Hash the msg -- no need for msg number..? 
bool FirstTimeMsg(const std::string& msg, MsgNum msgNumber, bool storyMode = true);

// Necessary ?
bool HasShownMsg(MsgNum msgNumber);

// Show message the first time it is triggered for the player.
// But reset the flag each session - i.e. stored locally, not on server. 
bool FirstTimeMsgThisSession(const std::string& msg, MsgNum msgNumber, bool storyMode = true);
}

#endif

