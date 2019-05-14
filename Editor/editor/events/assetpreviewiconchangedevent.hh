
#pragma once

#include <editor/editorexport.hh>
#include <cobalt/core/csevent.hh>
#include <cobalt/core/csresourcelocator.hh>

#include <editor/events/assetpreviewiconchangedevent.refl.hh>


CS_CLASS()
class AssetPreviewIconChangedEvent : public CS_SUPER(csEvent)
{
  CS_CLASS_GEN;
public:
  AssetPreviewIconChangedEvent(const csResourceLocator &locator);

  const csResourceLocator &GetLocator() const;

private:
  csResourceLocator m_locator;

};