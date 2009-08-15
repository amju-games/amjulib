#include "EventPoller.h"
#include "AmjuAssert.h"

namespace Amju
{
void EventPoller::AddListener(EventListener* pListener)
{
  Assert(pListener);
  m_listeners.insert(pListener);
}

void EventPoller::RemoveListener(EventListener* pListener)
{
  m_listeners.erase(pListener);
}

bool EventPoller::HasListener(EventListener* pListener) const
{
  return (m_listeners.find(pListener) != m_listeners.end());
}

void EventPoller::SetImpl(EventPollerImpl* pImpl)
{
  m_pImpl = pImpl;
}

void EventPoller::Update()
{
  Assert(m_pImpl.GetPtr());
  m_pImpl->Update(&m_listeners);
}
}
