#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/csentitystatecsfloader.refl.hh>

class csEntityState;

CS_CLASS()
class CSE_API csEntityStateCSFLoader : public CS_SUPER(csBaseCSFLoader)
{
  CS_CLASS_GEN_OBJECT;
public:
  csEntityStateCSFLoader();
  virtual ~csEntityStateCSFLoader();

  virtual bool CanLoad(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual iObject *Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;

private:
  void LoadProperty(csEntityState *entityState, const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const;
};

