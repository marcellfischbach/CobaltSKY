#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>

#include <cobalt/entity/blueprint/csblueprint.refl.hh>

class csBPEntity;
class csBPEntityState;
class csEntity;
class csEntityState;

CS_CLASS()
class CSE_API csBlueprint : public iObject
{
  CS_CLASS_GEN_OBJECT;
public:
  csBlueprint();
  virtual ~csBlueprint();

  csEntity *CreateEntity() const;
  csEntityState *CreateEntityState() const;

  void SetEntity(csBPEntity *entity);
  void SetEntityState(csBPEntityState *entityState);


private:

  csBPEntity *m_entity;
  csBPEntityState *m_entityState;
};

