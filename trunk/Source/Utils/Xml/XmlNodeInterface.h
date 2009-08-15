/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: XmlNodeInterface.h,v $
Revision 1.3  2007/10/21 19:25:20  jay
Copy improved XML lib from Pool

Revision 1.1.10.4  2007/06/05 08:27:36  jay
Replace old XML parser with new third party version

Revision 1.1.10.3  2007/06/04 21:33:59  jay
Replace XML parser with better third party implementation

Revision 1.1.10.2  2006/08/06 08:26:24  jay
Added useful typedef, PXml

Revision 1.1.10.1  2005/10/28 12:42:06  jay
Add GetChild(int); hopefully fix so parsing bad xml doesn't get stuck in a loop.

Revision 1.1  2004/09/08 15:43:23  jay
Added to repository
  
*/

#if !defined(ENT_XML_NODE_INTERFACE_H_INCLUDED)
#define ENT_XML_NODE_INTERFACE_H_INCLUDED

#include <string>
#include "XmlParser2.h" // third-party XML parser

namespace Amju
{
typedef XMLNode  PXml;

PXml ParseXml(const char* xmlInput);
}

#endif
