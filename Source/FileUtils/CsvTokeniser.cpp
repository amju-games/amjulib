/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: CsvTokeniser.cpp,v $
Revision 1.2  2008/05/08 10:58:03  jay
New memory management code

Revision 1.1  2004/09/08 15:43:17  jay
Added to repository
  
*/

// This source code originally written by JASON COLMAN 2000-2003. 
// You may use it as you wish, at your own risk!  jason@amju.com.

#include "AmjuFirst.h"
#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "CsvTokeniser.h"
#include "AmjuAssert.h"
#include "StringUtils.h"
#include "AmjuFinal.h"

using namespace std;

namespace Amju
{
bool CsvTokeniser::Tokenise(std::string* pTail, std::string* pHead)
{
  AMJU_CALL_STACK;

  bool inQuote = false;

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

    // Handle commas in quotes.
    if (c == '"')
    {
      inQuote = !inQuote;
    }
    if (inQuote) // Handle commas in quotes.
    {
      continue;
    }

    if (c == ',')
    {
      *pHead = pTail->substr(0, i - 1);
      *pTail = pTail->substr(i);
      Trim(pHead);
      Trim(pTail);
      *pHead = Remove(*pHead, '"');
      return true;
    }
  }
  *pHead = *pTail;
  *pHead = Remove(*pHead, '"'); // ?
  *pTail = "";
  return true;
}
}

