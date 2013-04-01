/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: XmlTestMain.cpp,v $
Revision 1.3  2008/05/08 10:58:12  jay
New memory management code

Revision 1.2  2007/10/21 19:25:21  jay
Copy improved XML lib from Pool

Revision 1.1.10.1  2007/06/04 21:34:00  jay
Replace XML parser with better third party implementation

Revision 1.1  2004/09/08 15:43:24  jay
Added to repository
  
*/


#include <AmjuFirst.h>
#if defined(WIN32)
#pragma warning(disable:4786)
#endif

#include <iostream>
#include <string>
#include <vector>
#include "XmlNodeInterface.h"
#include <AmjuFinal.h>

using namespace Amju;

void usage()
{
  AMJU_CALL_STACK;

    std::cout << "xmlTest - parses XML text.\n"
            " This is used to make sure the parser works correctly.\n\n"
            "Usage: xmlTest <xml string>\n";
}

int main(int argc, char** argv)
{
  AMJU_CALL_STACK;

    // ONE XML file is specified as input.
    if (argc < 2)
    {
        usage();
        return 1;
    }


    std::string str(argv[1]);

    //XmlNodeInterface* pXml = file.getRootNode();

    PXml node = ParseXml(str.c_str());


    return 0;
}

