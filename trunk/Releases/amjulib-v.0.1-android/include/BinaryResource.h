#ifndef RESOURCE_FILE_H
#define RESOURCE_FILE_H

#include "ResourceManager.h"
#include "BinaryData.h"

namespace Amju
{
// Raw binary data resource type, e.g. wavs, mods
class BinaryResource : public Resource
{
public:
  bool Load(const std::string& filename);
  const unsigned char* GetData() const;
  unsigned int GetSize() const;

protected:
  BinaryData m_data;
};

Resource* BinaryResourceLoader(const std::string& resName);

}

#endif
