#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/csentitycsfloader.refl.hh>



CS_CLASS()
class CSE_API csEntityCSFLoader : public CS_SUPER(csBaseCSFLoader)
{
  CS_CLASS_GEN_OBJECT;
public:
  csEntityCSFLoader();
  virtual ~csEntityCSFLoader();

  virtual bool CanLoad(const csfEntry *entry, const csResourceLocator &locator, cs::iObject *userData = nullptr) const;
  virtual const cs::Class *EvalClass(const csfEntry *entry, const csResourceLocator &locator, cs::iObject *userData = nullptr) const;
  virtual csResourceWrapper *Load(const csfEntry *entry, const csResourceLocator &locator, cs::iObject *userData = nullptr) const;

};

