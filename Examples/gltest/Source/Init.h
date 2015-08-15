#pragma once

namespace Amju
{
// Call once at the start of the program to initialise GameObjectFactory
//  and Collision functions. 
// There is a nifty trick for doing this kind of initialisation in 
//  "Modern C++ Design", where a global variable is initialised by calling
//  a function, which has the side effect of doing the initialisation you
//  want. Unfortunately this nifty trick does not work inside libraries.
//  For Android NDK builds, it currently causes a crash in LoadLibrary.

void Init();
}

