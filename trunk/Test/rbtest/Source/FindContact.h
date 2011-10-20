#ifndef FIND_CONTACT_H
#define FIND_CONTACT_H

#include "RBBox.h"

namespace Amju
{
bool FindContact(const RBBox& box1, const RBBox& box2, Contact* c);
}

#endif

