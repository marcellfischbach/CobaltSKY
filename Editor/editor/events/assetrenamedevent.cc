
#include <editor/events/assetrenamedevent.hh>

AssetRenamedEvent::AssetRenamedEvent(const cs::ResourceLocator &from, const cs::ResourceLocator &to, ResourceFetchMode mode, cs::iObject *object)
  :cs::Event()
  , m_from(from)
  , m_to(to)
  , m_mode(mode)
  , m_object(object)
{

}

const cs::ResourceLocator &AssetRenamedEvent::GetFrom() const
{
  return m_from;
}

const cs::ResourceLocator &AssetRenamedEvent::GetTo() const
{
  return m_to;
}

AssetRenamedEvent::ResourceFetchMode AssetRenamedEvent::GetFetchMode() const
{
  return m_mode;
}

const cs::iObject *AssetRenamedEvent::GetObject() const
{
  return m_object;
}

