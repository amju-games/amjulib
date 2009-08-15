/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: GlueFile.h,v $
Revision 1.4  2007/07/04 18:40:44  jay
Merge ref-counting from AMJU_POOL branch.
We must ref count GlueFileBinaryData so we only delete allocated data when
 there are no more copies.

Revision 1.3  2006/06/27 18:16:01  jay
Store filename, as there may now be multiple glue files.
Del unused files: create sh script as well as .bat script

Revision 1.2  2006/02/17 18:59:08  jay
Added new GetBinary() overload. This returns an _object_ containing a
pointer to a buffer. The buffer can be allocated by the object, which then
owns the buffer; or, the pointer can point to memory allocated by
something else.

Revision 1.1  2004/09/08 15:43:19  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_GLUE_FILE_H_INCLUDED)
#define SCHMICKEN_GLUE_FILE_H_INCLUDED

#include "Common.h"
#include "SharedPtr.h"
#include <fstream>
#include <list>
#include <map>
#include <string>
#include <utility> // pair
#include <vector>

namespace Amju
{
// A GlueFile is an archive, composed of many other sub-files.
// Like a zip file, for example. But there's no compression, although the
// sub-files may of course be compressed.
//
//
/*

GlueFile format

At the start of the GlueFile, the number of sub-files and the position of a table
is stored. The table lists the sub-file names (name only; no paths are allowed 
in GlueFiles). Also in the table is the size of the sub-file.
The table is at the end of the GlueFile, after all the sub-files.
    
<uint32> Number of files in archive
<uint32> Position in file of start of filetable
<file 0>
<file 1>    
.
.
<file n>

Following the last sub-file is the table. The format is as follows, for each
sub-file.

<uint32> length of sub-filename n
<bytes> name of sub-file n
<uint32> size of sub-file n


As uint32s are used, this constrains the max size of the GlueFile.

*/

// Stores binary data loaded from a Glue File.
// Can allocate a buffer, or just store a pointer to a buffer owned by 
// something else. If Allocate() is called, we allocate a buffer and delete
// if in the dtor.
class GlueFileBinaryData
{
public:
  GlueFileBinaryData();
  ~GlueFileBinaryData();

  // Override copying to maintain ref count
  GlueFileBinaryData& operator=(const GlueFileBinaryData& rhs);
  GlueFileBinaryData(const GlueFileBinaryData& rhs);

  void Allocate(uint32 size);
  unsigned char* const GetBuffer();
  void SetBuffer(unsigned char* pBuf);

private:
  // Functions to maintain ref count
  void IncCount();
  void DecCount();
  void Copy(const GlueFileBinaryData& );

private:
  unsigned char* m_pBuf;
  bool m_allocated;
  // This must be ref counted, or the first call to the dtor will
  //  delete the buffer if allocated. 
  int m_refCount;
};

class GlueFile : public Shareable
{
public:
  GlueFile();
  virtual ~GlueFile();

  // Use this to start making a new Glue file.
  // NOT used for reading from a Glue file.
  bool CreateGlueFile(const std::string gluefilename);

  // Open the GlueFile, either for writing (i.e. add another sub-file)
  // or for reading, (i.e. retrieve a sub-file). 
  virtual bool OpenGlueFile(const std::string gluefilename, bool read);

  std::string GetFilename() const;

  // Add a file to the GlueFile, identifying it by its filename.
  // NB Could the filename have a path prepended ?
  bool AddItem(const std::string& filename);

  // Gets listing of sub-files in the GlueFile.
  // Inserts the sub-file names into the given vector.
  bool Dir(std::vector<std::string>* pResult);

  // Get the start position of a Glued file. To seek to a position in 
  // the glued file, seek to the position plus the value returned from
  // this function.
  // Returns false (fails) if subfilename not recognised.
  bool GetSeekBase(const std::string& subfilename, uint32* pResult);

  // Get sub-file size
  uint32 GetSize(const std::string& subfilename) const;

  // Get/set the absolute position in the Glue file. 
  // This is used to enable a stack of positions.
  virtual uint32 GetPos();
  virtual void SetPos(uint32 pos);

  // Read some data from the current absolute position in the Glue file.
  virtual uint32 GetBinary(uint32 numbytes, unsigned char* pBuffer);

  // Read some data from the given absolute position, _not_ the position
  // stored by this object.
  virtual uint32 GetBinary(uint32 seekPos, uint32 numbytes, unsigned char* pBuffer);

  // Get binary data back in an object.
  // Subclasses of GlueFile can return a different type of object, which
  // may be more efficient than copying the binary data.
  virtual GlueFileBinaryData GetBinary(uint32 seekPos, uint32 numbytes);

  // Print out sub-files whose names are not passed to GetSeekBase().
  // This was supposed to happen in dtor but this doesn't seem to get called.
  void PrintUnused();

protected:

  // Just open the file: used by OpenGlueFile(), which may be overridden to
  // read in the file too.
  bool SimpleOpen(const std::string gluefilename, bool read);

  // Write the table at the end of the GlueFile.
  bool WriteTable();

  // Get the filetable into memory before we overwrite it.
  bool ReadTable();

protected:
  // The Glue (archive) File
  std::fstream m_gluefile; 

  std::string m_filename;

  // Map of sub-file names to sizes and positions. I.e. this is the table
  // used by WriteTable() and ReadTable().
  typedef std::pair<uint32, uint32> SizePos;
  typedef std::map<std::string, SizePos> FileTable;
  FileTable m_table;
  // But we don't want the table reordered! So we use a list of filenames
  // to keep the order in which the sub-files have been added.
  typedef std::list<std::string> FileList;
  FileList m_filenames;

  // Position of table in file.
  uint32 m_tablePos;

  // Keep a list of sub-files which are in the glue file but which are never 
  // used. This is so old, unused data can be easily identified.
  // The dtor writes out this list.
  FileList m_unusedFiles;
};
}

#endif

