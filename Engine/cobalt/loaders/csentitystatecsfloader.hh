#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/csentitystatecsfloader.refl.hh>

namespace cs
{
class EntityState;


CS_CLASS()
class CSE_API EntityStateCSFLoader : public CS_SUPER(cs::BaseCSFLoader)
{
  CS_CLASS_GEN_OBJECT;
public:
  EntityStateCSFLoader();
  virtual ~EntityStateCSFLoader();

  virtual bool CanLoad(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData = nullptr) const;
  virtual const cs::Class *EvalClass(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData = nullptr) const;
  virtual cs::ResourceWrapper *Load(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData = nullptr) const;

private:
  void LoadProperty(cs::EntityState *entityState, const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const;
};

}
