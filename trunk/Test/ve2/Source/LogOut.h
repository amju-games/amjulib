#ifndef LOG_OUT_H_INCLUDED
#define LOG_OUT_H_INCLUDED

namespace Amju
{
// Send logout mesg to server for current player. Call when session ends and on process exit
void SendLogOut();
}

#endif
