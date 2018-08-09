
#include <editor/events/assetrenamedevent.hh>

AssetRenamedEvent::AssetRenamedEvent(const csResourceLocator &from, const csResourceLocator &to, ResourceFetchMode mode, iObject *object)
  :csEvent()
  , m_from(from)
  , m_to(to)
  , m_mode(mode)
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

AssetRenamedEvent::ResourceFetchMode AssetRenamedEvent::GetFetchMode() const
{
  return m_mode;
}

const iObject *AssetRenamedEvent::GetObject() const
{
  return m_object;
}

