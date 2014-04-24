#ifndef ROOM_H_INCLUDED
#define ROOM_H_INCLUDED

#include "Ve1Object.h"
#include "Tile.h"

namespace Amju
{
class Room : public Ve1Object
{
public:
  Room();

  virtual void Update();
  virtual bool Load(File*);

  static const char* TYPENAME;
  virtual const char* GetTypeName() const;
  virtual void SetEditMenu(GuiMenu*) {} // TODO chop this 

  virtual void OnLocationEntry();

  enum RoomDir { AMJU_ROOM_N, AMJU_ROOM_S, AMJU_ROOM_E, AMJU_ROOM_W };
  int GetDest(RoomDir) const;

  Vec2f GetSize() const;

private:
  bool LoadGrid(int i, File* f);

private:
  friend class RoomNode;

  Strings m_texNames;

  // All tiles. 
  typedef std::vector<Tile> TileVec;
  static const int NUM_TILE_MAPS = 2;
  TileVec m_tiles[2];

  // Multiple tile vectors:
  // Map 0: background images - do not interact
  // Map 1: obstacles: tiles which cannot be traversed
  // TODO maybe:
  // Map 2: static baddies ?
  // Map 3: static food??

  Vec2f m_tilesize;
  Vec2i m_gridsize;

  // Four destination rooms to N, S, E, W
  int m_dest[4];

  std::string m_music;
};

// Get the current Room object
Room* GetRoom();

// Get filename for the given room location number
std::string MakeRoomFilename(int loc);

}

#endif
