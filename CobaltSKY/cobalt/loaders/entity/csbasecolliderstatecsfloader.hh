#pragma once
#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/entity/cscolliderstatecsfloader.hh>
#include <cobalt/loaders/entity/csbasecolliderstatecsfloader.refl.hh>

CS_CLASS()
class CSE_API csBaseColliderStateCSFLoader : public CS_SUPER(csColliderStateCSFLoader)
{
  CS_CLASS_GEN;
public:
  csBaseColliderStateCSFLoader();
  virtual ~csBaseColliderStateCSFLoader();

  virtual iObject *Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *GetLoadingClass() const;

};
