#pragma once
#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/entity/csbasecolliderstatecsfloader.hh>
#include <cobalt/loaders/entity/csstaticcolliderstatecsfloader.refl.hh>

CS_CLASS()
class CSE_API csStaticColliderStateCSFLoader : public CS_SUPER(csBaseColliderStateCSFLoader)
{
  CS_CLASS_GEN;
public:
  csStaticColliderStateCSFLoader();
  virtual ~csStaticColliderStateCSFLoader();

  virtual iObject *Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *GetLoadingClass() const;

};
