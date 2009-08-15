/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: BillBoard.h,v $
Revision 1.2  2007/01/23 10:04:52  jay
Made Save() public

Revision 1.1  2004/09/08 15:42:45  jay
Added to repository
  
*/

#if !defined(BILLBOARD_H_INCLUDED)
#define BILLBOARD_H_INCLUDED

#include "SolidLeaf.h"
namespace Amju
{
class Texture;

// A billboard Leaf always faces the camera directly.
class BillBoard : public SolidLeaf
{
public:
  BillBoard();

  virtual void Draw();
  virtual bool Load(const std::string& filename);

  // Load() above calls this one. Override this to load more data in subclasses.
  virtual bool Load(File* pf);

  virtual int GetNumberOfPolygons() const { return 2; }
  virtual bool LineIntersects(const Mgc::Segment3& seg, float r) const { return false; }

  virtual void CreateBoundingSphere(const Matrix& cm);

  virtual void StoreHeights(const Matrix&) {}
  virtual void AddHeights(HeightServer*, const BoundingSphere&) {}

  void SetSize(float size) { m_size = size; }
  void SetVertical(bool vertical) { m_vertical = vertical; }

protected:
  Texture* m_pTexture;
  // The size is the size from the origin to the edge, i.e. like the radius.
  float m_size;
  // If true, the billboard is always vertical; othewise can be non-vertical to
  // face the camera.
  bool m_vertical;

#if defined(SCENE_EDITOR)
public:
  virtual bool Save(bool recursive = false);
#endif
};
}
#endif

