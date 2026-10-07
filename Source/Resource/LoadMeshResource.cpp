// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026

#include "precomp.h" // first include

#include <AmjuFirst.h>
#include "LoadMeshResource.h"
#include "ObjMesh.h"
#include "File.h"
#include <AmjuFinal.h>

namespace Amju
{
ObjMesh* LoadMeshResource(File* f)
{
  std::string s;
  if (!f->GetDataLine(&s))
  {
    f->ReportError("Expected mesh file name");
    return 0;
  }
  ObjMesh* mesh = (ObjMesh*)TheResourceManager::Instance()->GetRes(s);
  Assert(mesh);
  return mesh;
}
}
