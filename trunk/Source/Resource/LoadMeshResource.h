#ifndef LOAD_MESH_RESOURCE_H
#define LOAD_MESH_RESOURCE_H

namespace Amju
{
class ObjMesh;
class File;

// Convenience function: read a line of text from a file, get the Resource
// and return it, cast to an ObjMesh*
ObjMesh* LoadMeshResource(File*); 
}

#endif


