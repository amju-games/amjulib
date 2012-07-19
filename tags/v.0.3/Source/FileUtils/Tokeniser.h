/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Tokeniser.h,v $
Revision 1.1  2004/09/08 15:43:20  jay
Added to repository
  
*/

#if !defined(TOKENISER_H_INCLUDED)
#define TOKENISER_H_INCLUDED

#include <string>

namespace Amju
{
// String chopper, with basic default behaviour.
class Tokeniser
{
public:
  // Chop up a string into a head and tail. 
  // pTail points to the original string before Tokenising.
  // If successful, the tail string has its first token removed. The string
  // pointed to by pHead is set to contain this token.
  virtual bool Tokenise(std::string* pTail, std::string* pHead);
};
}
#endif

