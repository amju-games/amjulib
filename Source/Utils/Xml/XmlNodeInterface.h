// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026
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
