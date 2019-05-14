#pragma once


#include <editor/editorexport.hh>
#include <cobalt/core/csevent.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <editor/events/assetrenamedevent.refl.hh>


CS_CLASS()
class AssetRenamedEvent : public CS_SUPER(csEvent)
{
  CS_CLASS_GEN;
  enum ResourceFetchMode
  {
    eRFM_FetchAll,
    eRFM_FetchNamedEntry,
  };
public:
  AssetRenamedEvent(const csResourceLocator &from, const csResourceLocator &to, ResourceFetchMode mode = eRFM_FetchAll, cs::iObject *object = nullptr);

  const csResourceLocator &GetFrom() const;
  const csResourceLocator &GetTo() const;
  ResourceFetchMode GetFetchMode() const;
  const cs::iObject *GetObject() const;

private:

  csResourceLocator m_from;
  csResourceLocator m_to;
  ResourceFetchMode m_mode;
  cs::iObject *m_object;


};

