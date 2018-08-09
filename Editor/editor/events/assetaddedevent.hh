#pragma once


#include <editor/editorexport.hh>
#include <cobalt/core/csevent.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <editor/events/assetaddedevent.refl.hh>


CS_INTERFACE()
class AssetAddedEvent : public CS_SUPER(csEvent)
{
  CS_CLASS_GEN;
public:
  AssetAddedEvent(const csResourceLocator &locator, iObject *object = 0);

  const csResourceLocator &GetLocator() const;
  const iObject *GetObject() const;

private:

  csResourceLocator m_locator;
  iObject *m_object;


};

