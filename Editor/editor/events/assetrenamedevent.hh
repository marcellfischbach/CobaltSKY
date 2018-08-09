#pragma once


#include <editor/editorexport.hh>
#include <cobalt/core/csevent.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <editor/events/assetrenamedevent.refl.hh>


CS_INTERFACE()
class AssetRenamedEvent : public CS_SUPER(csEvent)
{
  CS_CLASS_GEN;
  enum ResourceFetchMode
  {
    eRFM_FetchAll,
    eRFM_FetchNamedEntry,
  };
public:
  AssetRenamedEvent(const csResourceLocator &from, const csResourceLocator &to, ResourceFetchMode mode = eRFM_FetchAll, iObject *object = 0);

  const csResourceLocator &GetFrom() const;
  const csResourceLocator &GetTo() const;
  ResourceFetchMode GetFetchMode() const;
  const iObject *GetObject() const;

private:

  csResourceLocator m_from;
  csResourceLocator m_to;
  ResourceFetchMode m_mode;
  iObject *m_object;


};

