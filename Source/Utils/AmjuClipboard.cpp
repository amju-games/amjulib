// * Amjulib *
// (c) Copyright 2018 Jason Colman

#ifdef WIN32
#include <Windows.h>
#endif
#include "AmjuClipboard.h"

namespace Amju
{
bool CopyToClipboard(const std::string& s)
{
  if (s.empty())
  {
    return true; // ??
  }

#ifdef WIN32
  // https://msdn.microsoft.com/en-us/library/windows/desktop/ms649016(v=vs.85).aspx#_win32_Copying_Information_to_the_Clipboard

  if (!OpenClipboard(NULL)) // Win32
  {
    return false;
  }

  EmptyClipboard(); // Win32

  HGLOBAL hglbCopy = GlobalAlloc(GMEM_MOVEABLE, s.size() + 1);
  if (hglbCopy == NULL)
  {
    CloseClipboard();
    return false;
  }

  LPTSTR  lptstrCopy = (LPTSTR) GlobalLock(hglbCopy);
  memcpy(lptstrCopy, &s[0], s.size());
  lptstrCopy[s.size() + 1] = 0;    // null character 
  GlobalUnlock(hglbCopy);

  // Place the handle on the clipboard. 
  SetClipboardData(CF_TEXT, hglbCopy);

  CloseClipboard();
  return true;

#else
  // Not WIN32
  return false;
#endif
}
}
