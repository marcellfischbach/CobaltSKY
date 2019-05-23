
#include <editor/events/assetaddedevent.hh>

AssetAddedEvent::AssetAddedEvent(const cs::ResourceLocator &locator, cs::iObject *object)
  :cs::Event()
  , m_locator(locator)
  , m_object(object)
{

}

const cs::ResourceLocator &AssetAddedEvent::GetLocator() const
{
  return m_locator;
}

const cs::iObject *AssetAddedEvent::GetObject() const
{
  return m_object;
}

