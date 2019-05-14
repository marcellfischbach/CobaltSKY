
#include <editor/events/assetaddedevent.hh>

AssetAddedEvent::AssetAddedEvent(const csResourceLocator &locator, cs::iObject *object)
  :csEvent()
  , m_locator(locator)
  , m_object(object)
{

}

const csResourceLocator &AssetAddedEvent::GetLocator() const
{
  return m_locator;
}

const cs::iObject *AssetAddedEvent::GetObject() const
{
  return m_object;
}

