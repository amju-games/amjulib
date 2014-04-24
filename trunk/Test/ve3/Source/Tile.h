#pragma once

#include <string>
#include <Vec2.h>
#include <Vec3.h>

namespace Amju
{
// Tile-based room data.
struct Tile
{
  Tile() {}
  Tile (const Vec3f& pos, const Vec2f& size, const Vec2i& grid, const std::string& tex, bool blend) : 
    m_pos(pos), m_size(size), m_grid(grid), m_tex(tex), m_blend(blend) {}

  Vec3f m_pos; 
  Vec2f m_size;
  Vec2i m_grid;
  std::string m_tex;
  bool m_blend;
};


}
