
#include <events/assetpreviewiconchangedevent.hh>

AssetPreviewIconChangedEvent::AssetPreviewIconChangedEvent(const csResourceLocator &locator)
  : csEvent ()
  , m_locator(locator)
{

}

const csResourceLocator &AssetPreviewIconChangedEvent::GetLocator() const
{
  return m_locator;
}