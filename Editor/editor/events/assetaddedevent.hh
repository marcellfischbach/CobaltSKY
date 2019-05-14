#pragma once


#include <editor/editorexport.hh>
#include <cobalt/core/csevent.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <editor/events/assetaddedevent.refl.hh>


CS_CLASS()
class AssetAddedEvent : public CS_SUPER(csEvent)
{
  CS_CLASS_GEN;
public:
  AssetAddedEvent(const csResourceLocator &locator, cs::iObject *object = 0);

  const csResourceLocator &GetLocator() const;
  const cs::iObject *GetObject() const;

private:

  csResourceLocator m_locator;
  cs::iObject *m_object;


};

