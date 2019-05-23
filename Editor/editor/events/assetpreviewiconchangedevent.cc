
#include <editor/events/assetpreviewiconchangedevent.hh>

AssetPreviewIconChangedEvent::AssetPreviewIconChangedEvent(const cs::ResourceLocator &locator)
  : cs::Event ()
  , m_locator(locator)
{

}

const cs::ResourceLocator &AssetPreviewIconChangedEvent::GetLocator() const
{
  return m_locator;
}