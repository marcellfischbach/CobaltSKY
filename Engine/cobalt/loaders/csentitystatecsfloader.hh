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

  virtual bool CanLoad(const csfEntry *entry, const csResourceLocator &locator, cs::iObject *userData = nullptr) const;
  virtual const cs::Class *EvalClass(const csfEntry *entry, const csResourceLocator &locator, cs::iObject *userData = nullptr) const;
  virtual csResourceWrapper *Load(const csfEntry *entry, const csResourceLocator &locator, cs::iObject *userData = nullptr) const;

private:
  void LoadProperty(csEntityState *entityState, const csfEntry *entry, const csResourceLocator &locator, cs::iObject *userData) const;
};

