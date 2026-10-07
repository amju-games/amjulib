// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026

#include "precomp.h" // first include
#include <AmjuFirst.h>
#if defined(MSVC)
#pragma warning(disable:4786)
#endif

#include <iostream>
#include <string.h>
#include "XmlNodeInterface.h"
#include <AmjuAssert.h>
#include <AmjuFinal.h>

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

    if (xe.error != eXMLErrorNone)
    {
      std::cout << "XML Result Error: " << XMLError(xe.error) << " line: " << xe.nLine << "\n";
    }

    //PrintTree(xMainNode, 0);

    return xMainNode;
}
}
