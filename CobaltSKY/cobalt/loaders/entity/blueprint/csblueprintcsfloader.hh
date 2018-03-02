#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/entity/blueprint/csblueprintcsfloader.refl.hh>


class csBlueprint;
class csBPEntity;
class csBPEntityState;
class csPropertySetter;

CS_CLASS()
class CSE_API csBlueprintCSFLoader : public CS_SUPER(csBaseCSFLoader)
{
  CS_CLASS_GEN_OBJECT;
public:
  csBlueprintCSFLoader();
  virtual ~csBlueprintCSFLoader();

  virtual bool CanLoad(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual iObject *Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;

private:
  void LoadEntity(csBlueprint *blueprint, const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const;
  void LoadEntityState(csBPEntity *entity, const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const;
  void LoadProperty(csBPEntityState *entityState, const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const;

  csPropertySetter* CreateProperty(const csfEntry *entry) const;
};
