/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: FileImpl.cpp,v $
Revision 1.3  2008/05/18 16:04:25  jay
MSVC update

Revision 1.2  2008/05/08 10:58:03  jay
New memory management code

Revision 1.1  2004/09/08 15:43:18  jay
Added to repository
  
*/

// This source code originally written by JASON COLMAN 2000-2003. 
// You may use it as you wish, at your own risk!  jason@amju.com.

#ifdef WIN32
#pragma warning(disable: 4786)
#endif
#include "AmjuFirst.h"
#include "FileImpl.h"
#include "AmjuFinal.h"

namespace Amju
{
FileImpl::~FileImpl()
{
  AMJU_CALL_STACK;

}
}
