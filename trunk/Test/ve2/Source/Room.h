#ifndef ROOM_H_INCLUDED
#define ROOM_H_INCLUDED

#include "Ve1Object.h"

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

  // Tile-based room data.
  // Group tiles by texture, to reduce state flipping.
  struct Tile
  {
    Tile() {}
    Tile (const Vec2f& pos, const Vec2f& size) : m_pos(pos), m_size(size) {}

    Vec2f m_pos; 
    Vec2f m_size;
  };
  typedef std::vector<Tile> TileVec;
  // For each texture, a vector of positions at which to draw a tile.
  typedef std::map<std::string, TileVec> TileMap;

  // Multiple tile maps:
  // Map 0: background images - do not interact
  // Map 1: obstacles: tiles which cannot be traversed

  // Map 2: static baddies ?
  // Map 3: static food??
  static const int NUM_TILE_MAPS = 2;
  TileMap m_tilemap[NUM_TILE_MAPS];

  Vec2f m_tilesize;
  Vec2i m_gridsize;

  // Four destination rooms to N, S, E, W
  int m_dest[4];
};

// Get the current Room object
Room* GetRoom();

// Get filename for the given room location number
std::string MakeRoomFilename(int loc);

}

#endif
