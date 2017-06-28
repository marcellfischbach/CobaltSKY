
#include <events/assetrenamedevent.hh>

AssetRenamedEvent::AssetRenamedEvent(const csResourceLocator &from, const csResourceLocator &to, iObject *object)
  :csEvent()
  , m_from(from)
  , m_to(to)
  , m_object(object)
{

}

const csResourceLocator &AssetRenamedEvent::GetFrom() const
{
  return m_from;
}

const csResourceLocator &AssetRenamedEvent::GetTo() const
{
  return m_to;
}

const iObject *AssetRenamedEvent::GetObject() const
{
  return m_object;
}

