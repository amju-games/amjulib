#ifndef CURSOR_MANAGER_H
#define CURSOR_MANAGER_H

#include "Singleton.h"
#include "Cursor.h"

namespace Amju
{
class CursorManager : public NonCopyable
{
private:
  CursorManager() {}
  friend class Singleton<CursorManager>;

public:
  bool Load();
  void Draw();
  void Update();
  Cursor* GetCursor(int i);

private:
  static const int NUM_CURSORS = 4;
  Cursor m_cursors[NUM_CURSORS];
};
typedef Singleton<CursorManager> TheCursorManager;
}

#endif
