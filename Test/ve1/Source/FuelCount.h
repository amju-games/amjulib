#ifndef FUEL_COUNT_H_INCLUDED
#define FUEL_COUNT_H_INCLUDED

namespace Amju
{
// Local player has local count of fuel cells picked up.
// On collision with spaceship, we transfer this number of cells to the overall 
//  total for the spaceship.

// Local Player count
void ResetLocalPlayerFuelCount();

int GetLocalPlayerFuelCount();

void ChangeLocalPlayerFuelCount(int delta);

}

#endif


