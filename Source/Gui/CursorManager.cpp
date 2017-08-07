#include <AmjuFirst.h>
#include "CursorManager.h"
#include "ResourceManager.h"
#include <AmjuFinal.h>

namespace Amju
{
CursorManager::CursorManager()
{
  for (int i = 0; i < NUM_CURSORS; i++)
  {
    m_cursors[i] = 0;
  }
}

// TODO This just returns first cursor. Maybe a bit field so we can tell if multiple cursors
//  are in the rect ?
bool CursorManager::IsCursorIn(const Rect& rect, int* cursorIndex) const
{
  for (int i = 0; i < NUM_CURSORS; i++)
  {
    Assert(m_cursors[i]);
    if (rect.IsPointIn(m_cursors[i]->GetPos()))
    {
      if (cursorIndex)
      {
        *cursorIndex = i;
      }
      return true;
    }
  }
  return false;
}

bool CursorManager::Load(const std::string& imageResource, const Vec2f& hotspot)
{
  for (int i = 0; i < NUM_CURSORS; i++)
  {
    m_cursors[i] = new Cursor;
    if (!m_cursors[i]->Load(imageResource, i, hotspot))
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
    m_cursors[i]->Draw();
  }
}

void CursorManager::Update()
{
}

Cursor* CursorManager::GetCursor(int i)
{
  Assert(i < NUM_CURSORS);
  return m_cursors[i];
}
}
