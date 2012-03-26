/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: XmlNodeInterface.cpp,v $
Revision 1.5  2008/05/08 10:58:11  jay
New memory management code

Revision 1.4  2007/11/06 21:48:54  jay
Don't assert if http result does not contain <?xml -- this can happen.
TODO Test this situation

Revision 1.3  2007/10/21 19:25:20  jay
Copy improved XML lib from Pool

Revision 1.1.10.4  2007/06/10 21:32:23  Administrator
Fix include path for MSVC

Revision 1.1.10.3  2007/06/05 08:27:36  jay
Replace old XML parser with new third party version

Revision 1.1.10.2  2007/06/04 21:33:59  jay
Replace XML parser with better third party implementation

Revision 1.1.10.1  2005/10/28 12:42:06  jay
Add GetChild(int); hopefully fix so parsing bad xml doesn't get stuck in a loop.

Revision 1.1  2004/09/08 15:43:23  jay
Added to repository
  
*/

#include "AmjuFirst.h"
#if defined(MSVC)
#pragma warning(disable:4786)
#endif

#include <iostream>
#include "XmlNodeInterface.h"
#include <AmjuAssert.h>
#include "AmjuFinal.h"

namespace Amju
{
void PrintTree(PXml p, int depth)
{
  AMJU_CALL_STACK;

  std::string sp(depth * 4, ' '); // indent string

  std::cout 
    << sp.c_str() << "Node: name: " << (p.getName() ? p.getName() : "<no name>")
    << " text:" << (p.getText() ? p.getText() : "<no text>")
    << " num children: " << p.nChildNode() << "\n";

  for (int i = 0; i < p.nChildNode(); i++)
  {
    PXml ch = p.getChildNode(i);
    PrintTree(ch, depth + 1);
  }
}

PXml ParseXml(const char* xmlInput)
{
  AMJU_CALL_STACK;

    // Find the <xml> tag, start parsing from there.
    const char* xmlTag = strstr(xmlInput, "<?xml");
    if (!xmlTag)
    {
#ifdef XML_DEBUG
      std::cout << "XML has no <xml> declaration: " << xmlInput << "\n";
#endif
      //Assert(0); 
      xmlTag = xmlInput;
    }

    XMLResults xe;
    XMLNode xMainNode = XMLNode::parseString(xmlTag, NULL, &xe);

    //std::cout << "XML Result: " << xe.error << " line: " << xe.nLine << "\n";
    //PrintTree(xMainNode, 0);

    return xMainNode;
}
}
