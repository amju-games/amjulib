#pragma once

#include "SpringSystem.h"

namespace Amju
{
// When we create the initial shape from a mesh, each vertex (particle) gets a spring from its bind pose position, which is
//  immovable, to its current position. The k value for these springs determines how squishy or tough the squishy is.
// When we push a vertex, a force due to the spring acts to move back against the push.
// When a vert moves, we should act to retain the volume of the shape overall.
// Step 1 is to work out the volume of the initial shape - must be closed?
// 
class Squishy : public SpringSystem
{
public:

};
}
