/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: CsvTokeniser.h,v $
Revision 1.1  2004/09/08 15:43:18  jay
Added to repository
  
*/

#if !defined(CSV_TOKENISER_H_INCLUDED)
#define CSV_TOKENISER_H_INCLUDED

#include "Tokeniser.h"
namespace Amju
{
// Tokeniser for CSV files.
// Splits string *pTail into characters before comma and after comma.
// If successful, pTail contains the characteres after the comma,
// and pHead contains the characters before the comma.
class CsvTokeniser : public Tokeniser
{
public:
  virtual bool Tokenise(std::string* pTail, std::string* pHead);
};
}

#endif

