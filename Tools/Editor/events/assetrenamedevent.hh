#pragma once


#include <cobalt/core/csevent.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <events/assetrenamedevent.refl.hh>


CS_INTERFACE()
class AssetRenamedEvent : public CS_SUPER(csEvent)
{
  CS_CLASS_GEN;
public:
  AssetRenamedEvent(const csResourceLocator &from, const csResourceLocator &to, iObject *object = 0);

  const csResourceLocator &GetFrom() const;
  const csResourceLocator &GetTo() const;
  const iObject *GetObject() const;

private:

  csResourceLocator m_from;
  csResourceLocator m_to;
  iObject *m_object;

};

