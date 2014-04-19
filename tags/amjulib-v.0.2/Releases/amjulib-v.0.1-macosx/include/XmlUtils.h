/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: XmlUtils.h,v $
Revision 1.1  2004/09/08 15:43:24  jay
Added to repository
  
*/

#if !defined(XML_UTILS_H_INCLUDED)
#define XML_UTILS_H_INCLUDED

#include <string>

namespace Amju
{
	/**
	 * a place to put the commonly recurring xml helper functions
	 */
	namespace XML
	{
        	bool findXMLChars(const std::string& s);
		void escapeXML(std::string* pString);
		std::string escapeXML(const std::string& s);
		std::string replaceIllegalChars(const std::string& s, const char replacementChar = '?');
	}
}
#endif
