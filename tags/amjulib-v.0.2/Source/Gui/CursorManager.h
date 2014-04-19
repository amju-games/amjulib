#ifndef CURSOR_MANAGER_H
#define CURSOR_MANAGER_H

#include <Singleton.h>
#include <Rect.h>
#include "Cursor.h"

namespace Amju
{
class CursorManager : public NonCopyable
{
private:
  CursorManager();
  friend class Singleton<CursorManager>;

public:
  bool Load(const Vec2f& hotspot);
  void Draw();
  void Update();
  Cursor* GetCursor(int i);

  // Returns true if a cursor is in the given rect. Can also return index of (first) cursor found.
  bool IsCursorIn(const Rect& rect, int* cursorIndex = 0) const;

private:
  static const int NUM_CURSORS = 4;
  Cursor* m_cursors[NUM_CURSORS];
};
typedef Singleton<CursorManager> TheCursorManager;
}

#endif
