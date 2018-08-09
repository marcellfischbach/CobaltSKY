
#pragma once

#include <cobalt/core/csevent.hh>
#include <cobalt/core/csresourcelocator.hh>

#include <events/assetpreviewiconchangedevent.refl.hh>


CS_INTERFACE()
class AssetPreviewIconChangedEvent : public CS_SUPER(csEvent)
{
  CS_CLASS_GEN;
public:
  AssetPreviewIconChangedEvent(const csResourceLocator &locator);

  const csResourceLocator &GetLocator() const;

private:
  csResourceLocator m_locator;

};