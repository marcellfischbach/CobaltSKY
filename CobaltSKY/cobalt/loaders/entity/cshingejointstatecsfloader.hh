#pragma once
#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/entity/csjointstatecsfloader.hh>
#include <cobalt/loaders/entity/cshingejointstatecsfloader.refl.hh>



CS_CLASS()
class CSE_API csHingeJointStateCSFLoader : public CS_SUPER(csJointStateCSFLoader)
{
  CS_CLASS_GEN;
  csHingeJointStateCSFLoader();
  virtual ~csHingeJointStateCSFLoader();

  virtual iObject *Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *GetLoadingClass() const;

};
