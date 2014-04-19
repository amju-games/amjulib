#ifndef GS_OBJMESH_H_INCLUDED
#define GS_OBJMESH_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
class GSObjMesh : public GSGui
{
  GSObjMesh();
  friend class Singleton<GSObjMesh>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  void OnOKButton();
  void SetFile(const std::string& pathAndFile);

  void SetId(int id); // ID of Game Object for which we are setting mesh

  void OnFinishedUpload();  // called when all files uploaded ok

  void SetError(const std::string& error);

private:
  // We upload files to the server - track total and how many so far
  int m_uploadedFiles;
  int m_totalFiles;

  int m_objId; // ID of Game Object for which we are setting obj mesh

  std::string m_assetFilename;
  std::string m_assetFileContents;
  std::string m_dataFilename;
  std::string m_dataFileContents;
};
typedef Singleton<GSObjMesh> TheGSObjMesh;
} // namespace
#endif
