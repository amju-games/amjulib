/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Tokeniser.cpp,v $
Revision 1.2  2008/05/08 10:58:07  jay
New memory management code

Revision 1.1  2004/09/08 15:43:20  jay
Added to repository
  
*/

// This source code originally written by JASON COLMAN 2000-2003. 
// You may use it as you wish, at your own risk!  jason@amju.com.

#include "AmjuFirst.h"
#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Tokeniser.h"
#include "File.h" 
#include "AmjuAssert.h"
#include "StringUtils.h"
#include "AmjuFinal.h"

using namespace std;

namespace Amju
{
bool Tokeniser::Tokenise(string* pTail, string* pHead)
{
  AMJU_CALL_STACK;

  Assert(pTail);
  Assert(pHead);
  if (!pTail || !pHead)
  {
    return false;
  }

  Trim(pTail); 
  
  if (pTail->empty())
  {
    return false;
  }
  
  unsigned int i = 0;
  while (i < pTail->size())
  {
    char c = pTail->at(i);
    i++;
    if (c == ' ' || c == ':')
    {
      if (pTail->size() > 1)
      {
        *pHead = pTail->substr(0, i);
        *pTail = pTail->substr(i);
        Trim(pHead);
        Trim(pTail);
        return true;
      }
      else
      {
        Assert(0);
      }
    }
  }
  *pHead = *pTail;
  *pTail = "";
  return true;
}
}
