/*
Amju Games source code (c) Copyright Jason Colman 2010
*/

#ifdef WIN32
#if defined(_DEBUG)
#pragma comment(lib, "../../../../../Build/Debug/AmjuLibMsvc.lib")
#else
#pragma comment(lib, "../../../../../Build/Release/AmjuLibMsvc.lib")
#endif 
#endif // WIN32

#include <iostream>
#include "GlueFile.h"

using namespace Amju;
using namespace std;

namespace Amju
{	
void ReportError(const std::string& s)
{
  std::cout << s.c_str() << "\n";
}
}

void Usage()
{
  cout << "Usage:\n"
       << "glue -c <gluefile>          - create new glue file.\n"
       << "glue -a <gluefile> <files>  - adds file(s) to gluefile.\n"
       << "glue -p <gluefile> <files>  - replaces file(s) in gluefile.\n"
       << "glue -d <gluefile>          - lists all files in gluefile.\n"
       << "glue -s <gluefile> <file>   - output contents of file.\n";
}

int main(int argc, char* argv[])
{
  // Usage:
  // glue -c[reate] <filename>
  // - create a new glue file with the given filename.
  // glue -a[dd] <gluefile> <subfile>
  //  - glue subfile onto gluefile.
  // glue -p[atch] <gluefile> <subfile>
  //  - replace subfile in gluefile.
  // glue -d[ir] <gluefile>
  //  - list contents of gluefile

  if (argc < 3)
  {
    Usage();
    return 0;
  }

  std::string option = argv[1];

  if (option == "-c")
  {
    if (argc != 3)
    {
      Usage();
      return -1;
    }

    GlueFile gf;
    if (gf.CreateGlueFile(argv[2]))
    {
      cout << "Created glue file " << argv[2] << endl;
      return 0;
    }
  }

  if (option == "-d")
  {
    if (argc != 3)
    {
      Usage();
      return -1;
    }

    GlueFile gf;
    if (!gf.OpenGlueFile(argv[2], false)) // false means write access
    {
      cout << "Failed to open glue file " << argv[2] << endl;
      return -1;
    }

    cout << "Listing of glue file " << argv[2] << ":" << endl;
    vector<string> v;
    gf.Dir(&v);
    for (unsigned int i = 0; i < v.size(); i++)
    {
      cout << v[i] << endl;
    }
    cout << v.size() << " files.\n";
    return 0;
  }

  if (option == "-a" || option == "-p")
  {
    if (argc < 4)
    {
      Usage();
      return -1;
    }

    GlueFile gf;
    if (!gf.OpenGlueFile(argv[2], false)) // false means write access
    {
      cout << "Failed to open glue file " << argv[2] << endl;
      return -1;
    }

    for (int i = 3; i < argc; i++)
    {
      // Add file to glue file 
      bool ok = false;
      if (option == "-a") // add
      {
        ok = gf.AddItem(argv[i]); 
      }
      else // patch
      {
        ok = gf.PatchItem(argv[i]);
      }

      if (ok)
      {
        cout << "Added file " << argv[i] << " ok\n";
      }
      else
      {
        cout << "Failed to add " << argv[i] << " to glue file " << argv[2] << endl;
        return -1;
      }
    }
    return 0;
  }

  if (option == "-s")
  {
    if (argc != 4)
    {
      Usage();
      return -1;
    }

    GlueFile gf;
    if (!gf.OpenGlueFile(argv[2], false)) // false means write access
    {
      cout << "Failed to open glue file " << argv[2] << endl;
      return -1;
    }

    std::string subfile = argv[3];
    cout << "Stat file " << subfile << ":" << endl;
    unsigned int seekbase = 0;
    if (!gf.GetSeekBase(subfile, &seekbase))
    {
      cout << "Failed to find subfile " << subfile << endl;
      return -1;
    }
    uint32 size = gf.GetSize(subfile);
    unsigned char* buf = new unsigned char[size + 1];
    gf.GetBinary(seekbase, size, buf);
    for (unsigned int i = 0; i < size; i++)
    {
      std::cout << buf[i];
    }
    std::cout << "\nEnd of stat\n";

    return 0;
  }

  Usage();
  return 0;  
}
