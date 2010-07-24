#include "BinaryResource.h"
#include "File.h"
#include "AmjuAssert.h"

namespace Amju
{
Resource* BinaryResourceLoader(const std::string& resName)
{
  std::string filename = File::GetRoot() + resName;

  BinaryResource* rf = new BinaryResource;

  if (!rf->Load(filename))
  {
    delete rf;
    return 0;
  }

  return rf;
}

bool BinaryResource::Load(const std::string& filename)
{
  // TODO Use File class

  FILE *f = fopen(filename.c_str(), "rb");

  if (!f) 
  {
    return false;
  }

  fseek (f , 0, SEEK_END);
  size_t size = ftell (f);
  rewind(f);

  // allocate memory to contain the whole file:
  m_data.resize(size);

  // copy the file into the buffer:
  size_t result = fread(&m_data[0], 1, size, f);
  if (result != size) 
  { 
    perror ("Error reading file"); 
    Assert(0);
  }

  fclose(f);

  return true;
}

const unsigned char* BinaryResource::GetData() const
{
  if (m_data.empty())
  {
    return 0;
  }

  return &m_data[0];
}

unsigned int BinaryResource::GetSize() const
{
  return m_data.size();
}
}
