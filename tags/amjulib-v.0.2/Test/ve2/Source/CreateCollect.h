#ifndef CREATE_COLLECT_H_INCLUDED
#define CREATE_COLLECT_H_INCLUDED

namespace Amju
{
// Called when we get server responses. So server says "I have given this player 5 collects".
// We cerate them as local (non-cached) Game Objects. If the player intersects them, we send
// special ID back to server to prove we collected a real Collect.
void CreateCollect(int type, int specialId);
 
}

#endif

