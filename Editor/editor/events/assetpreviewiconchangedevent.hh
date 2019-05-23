
#pragma once

#include <editor/editorexport.hh>
#include <cobalt/core/csevent.hh>
#include <cobalt/core/csresourcelocator.hh>

#include <editor/events/assetpreviewiconchangedevent.refl.hh>


CS_CLASS()
class AssetPreviewIconChangedEvent : public CS_SUPER(cs::Event)
{
  CS_CLASS_GEN;
public:
  AssetPreviewIconChangedEvent(const cs::ResourceLocator &locator);

  const cs::ResourceLocator &GetLocator() const;

private:
  cs::ResourceLocator m_locator;

};