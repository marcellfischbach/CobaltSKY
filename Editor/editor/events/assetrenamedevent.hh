#pragma once


#include <editor/editorexport.hh>
#include <cobalt/core/csevent.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <editor/events/assetrenamedevent.refl.hh>


CS_CLASS()
class AssetRenamedEvent : public CS_SUPER(cs::Event)
{
  CS_CLASS_GEN;
  enum ResourceFetchMode
  {
    eRFM_FetchAll,
    eRFM_FetchNamedEntry,
  };
public:
  AssetRenamedEvent(const cs::ResourceLocator &from, const cs::ResourceLocator &to, ResourceFetchMode mode = eRFM_FetchAll, cs::iObject *object = nullptr);

  const cs::ResourceLocator &GetFrom() const;
  const cs::ResourceLocator &GetTo() const;
  ResourceFetchMode GetFetchMode() const;
  const cs::iObject *GetObject() const;

private:

  cs::ResourceLocator m_from;
  cs::ResourceLocator m_to;
  ResourceFetchMode m_mode;
  cs::iObject *m_object;


};

