/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: XmlUtils.cpp,v $
Revision 1.3  2008/05/08 10:58:12  jay
New memory management code

Revision 1.2  2007/11/10 22:01:58  jay
Fixes for new LeafData2

Revision 1.1  2004/09/08 15:43:24  jay
Added to repository
  
*/

#include <AmjuFirst.h>
#ifdef MSVC
#pragma warning(disable: 4786)
#endif

#include "XMLUtils.h"
#include <string>
#include <vector>
#include <stdlib.h>
#include <AmjuFinal.h>

using namespace std;

namespace Amju
{
	namespace XML
	{
        bool findXMLChars(const string& s)
        {
  AMJU_CALL_STACK;

            return (s.find_first_of("&<>'\"") != string::npos);
        }

	void escapeXML(string* pString)
        {
  AMJU_CALL_STACK;

            if (!pString)
                return;
            *pString = escapeXML(*pString);
        }

	string escapeXML(const string& s)
        {
  AMJU_CALL_STACK;

            string escaped;
            // reserve the minimum size we know we will need
            if (s.size() > 0)
                escaped.reserve(s.size());
            for (string::const_iterator it = s.begin();
                 it != s.end();
                 ++it)
            {
                switch (*it)
                {
                    case '&':
                        escaped += "&amp;";
                        break;
                    case '<':
                        escaped += "&lt;";
                        break;
                    case '>':
                        escaped += "&gt;";
                        break;
                    case '"':
                        escaped += "&quot;";
                        break;
                    case '\'':
                        escaped += "&apos;";
                        break;
                    default:
                        escaped += *it;
                        break;
                }
            }
            return escaped;
        }

	string replaceIllegalChars(const string& s, const char replacementChar)
        {
  AMJU_CALL_STACK;

    		string cleanedXML(s);
    		const string illegalCharacterList;// = sysConfig->get("xml.illegalCharacters");
		    const vector<string> illegalCharacters;// = split(illegalCharacterList, ",");
		    vector<string>::const_iterator illegalCharacter = illegalCharacters.begin();
		    while (illegalCharacter != illegalCharacters.end())
		    {
			    char illChar = atoi((*illegalCharacter).c_str());
			    for (unsigned index = 0; index < cleanedXML.length(); ++index)
			    {
				    if (cleanedXML[index] == illChar)
					    cleanedXML[index] = replacementChar;
			    }		
			    ++illegalCharacter;
		    }
            return cleanedXML;
        }
}
}
