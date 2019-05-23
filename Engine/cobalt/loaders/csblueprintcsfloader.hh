#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/csblueprintcsfloader.refl.hh>


namespace cs
{
class BPEntity;
class BPEntityState;
class PropertySetter;
class Blueprint;

CS_CLASS()
class CSE_API BlueprintCSFLoader : public CS_SUPER(cs::BaseCSFLoader)
{
  CS_CLASS_GEN_OBJECT;
public:
  BlueprintCSFLoader();
  virtual ~BlueprintCSFLoader();

  virtual bool CanLoad(const csfEntry *entry, const cs::ResourceLocator &locator, cs::iObject *userData = nullptr) const;
  virtual const cs::Class *EvalClass(const csfEntry *entry, const cs::ResourceLocator &locator, cs::iObject *userData = nullptr) const;
  virtual cs::ResourceWrapper *Load(const csfEntry *entry, const cs::ResourceLocator &locator, cs::iObject *userData = nullptr) const;

private:
  void LoadEntity(cs::Blueprint *blueprint, const csfEntry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const;
  void LoadEntityState(cs::BPEntity *entity, const csfEntry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const;
  void LoadProperty(cs::BPEntityState *entityState, const csfEntry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const;

  cs::PropertySetter* CreateProperty(const csfEntry *entry) const;
};

}
