#pragma once


#include <editor/editorexport.hh>
#include <cobalt/core/csevent.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <editor/events/assetaddedevent.refl.hh>


CS_CLASS()
class AssetAddedEvent : public CS_SUPER(cs::Event)
{
  CS_CLASS_GEN;
public:
  AssetAddedEvent(const cs::ResourceLocator &locator, cs::iObject *object = 0);

  const cs::ResourceLocator &GetLocator() const;
  const cs::iObject *GetObject() const;

private:

  cs::ResourceLocator m_locator;
  cs::iObject *m_object;


};

