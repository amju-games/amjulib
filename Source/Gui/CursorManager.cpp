#include "CursorManager.h"
#include "ResourceManager.h"

namespace Amju
{
bool CursorManager::Load()
{
  for (int i = 0; i < NUM_CURSORS; i++)
  {
    if (!m_cursors[i].Load(i))
    {
      return false;
    }
  }
  return true;
}

void CursorManager::Draw()
{
  for (int i = 0; i < NUM_CURSORS; i++)
  {
    m_cursors[i].Draw();
  }
}

void CursorManager::Update()
{
}

Cursor* CursorManager::GetCursor(int i)
{
  Assert(i < NUM_CURSORS);
  return &m_cursors[i];
}
}
