#pragma once
#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/entity/csspatialstatecsfloader.hh>
#include <cobalt/loaders/entity/csjointstatecsfloader.refl.hh>


CS_CLASS()
class CSE_API csJointStateCSFLoader : public CS_SUPER(csSpatialStateCSFLoader)
{
  CS_CLASS_GEN;
public:
  csJointStateCSFLoader();
  virtual ~csJointStateCSFLoader();

  virtual iObject *Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *GetLoadingClass() const;
};