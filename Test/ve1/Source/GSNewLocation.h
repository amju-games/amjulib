#ifndef GS_NEW_LOCATION_H_INCLUDED
#define GS_NEW_LOCATION_H_INCLUDED

#include <set>
#include <Singleton.h>
#include "GSGui.h"
#include "UploadReq.h"

namespace Amju
{
class GSNewLocation : public GSGui
{
public:
  GSNewLocation();
  virtual void OnActive();
  void OnOKButton();

  // Called when an UploadReq finishes
  void OnUploadFinished(const UploadInfo& ui);

  // Called when we get response for request for new location ID
  // ID is new, unique ID or empty on failure.
  void OnNewIdResponse(const std::string& id);

  void OnLocationCreated();
 
  void SetError(const std::string&);

  enum Mode { AMJU_ADD_NEW, AMJU_EDIT };
  void SetMode(Mode mode);

  void OnFinishedUpload();

protected:
  void RestoreLastPath();
  void RequestNewId();
  std::string MakeLocDir() const;

protected:
  // We upload files to the server - track total and how many so far
  int m_uploadedFiles;
  int m_totalFiles;

  typedef std::set<std::string> StrSet;
  // Pathname + Filename of files to upload for the new Location
  StrSet m_strs;

  std::string m_assetFilename;
  std::string m_dataFilename;
  std::string m_objFile;
  std::string m_locId; // ID for the new location (unique, from server)

  Mode m_mode;
};

typedef Singleton<GSNewLocation> TheGSNewLocation;
}

#endif

