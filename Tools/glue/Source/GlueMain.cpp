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
#include <GlueFile.h>

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
  cout << "Usage: glue -c <gluefile>         - create new glue file.\n"
       << "       glue -a <gluefile> <file>  - adds file to gluefile.\n"
       << "       glue -d <gluefile>         - lists all files in gluefile.\n";

}

int main(int argc, char* argv[])
{
  // Usage:
  // glue -c[reate] <filename>
  // - create a new glue file with the given filename.
  // glue -a[dd] <gluefile> <subfile>
  //  - glue subfile onto gluefile.
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

  if (option == "-a")
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

    if (gf.AddItem(argv[3]))
    {
      cout << "Added file " << argv[3] << " ok\n";
      return 0;
    }
    else
    {
      cout << "Failed to add " << argv[3] << " to glue file " << argv[2] << endl;
      return -1;
    }
  }

  Usage();
  return 0;  
}
