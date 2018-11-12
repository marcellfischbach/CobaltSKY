#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/csresourcewrapper.hh>

#include <cobalt/entity/blueprint/csblueprint.refl.hh>

class csBPEntity;
class csBPEntityState;
class csEntityWrapper;
class csEntityStateWrapper;

CS_CLASS()
class CSE_API csBlueprint : public iObject
{
  CS_CLASS_GEN_OBJECT;
public:
  csBlueprint();
  virtual ~csBlueprint();

  csEntityWrapper *CreateEntity() const;
  csEntityStateWrapper *CreateEntityState() const;

  void SetEntity(csBPEntity *entity);
  void SetEntityState(csBPEntityState *entityState);

private:

  csBPEntity *m_entity;
  csBPEntityState *m_entityState;
};

CS_CLASS()
class csBlueprintWrapper : public CS_SUPER(csResourceWrapper)
{
  CS_CLASS_GEN;
  CS_RESOURCE_WRAPPER(csBlueprint, csBlueprintWrapper, csResourceWrapper);
};
