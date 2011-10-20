#include "FindContact.h"

namespace Amju
{
bool FindContact(const RBBox& box1, const RBBox& box2, Contact* c)
{
  if (box1.FindContact(box2, c))
  {
    return true;
  }
  return box2.FindContact(box1, c);
}

}
