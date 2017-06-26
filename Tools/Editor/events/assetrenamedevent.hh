#pragma once


#include <cobalt/core/csevent.hh>
#include <cobalt/core/csresourcelocator.hh>


CS_CLASS()
class AssetRenamedEvent : public CS_SUPER(csEvent)
{
public:
  AssetRenamedEvent(const csResourceLocator &from, const csResourceLocator &to, iObject *objet = 0);

  const csResourceLocator &GetFrom() const;
  const csResourceLocator &GetTo() const;
  const iObject *GetObject() const;

private:

  csResourceLocator m_from;
  csResourceLocator m_to;
  iObject *m_object;

};