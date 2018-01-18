
#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>

#include <vector>

#include <cobalt/entity/blueprint/csbpentity.refl.hh>

class csBPEntityState;
class csBPBaseProperty;
class csEntity;
class csEntityState;


class CSE_API csBPEntity 
{
public:
  csBPEntity();
  ~csBPEntity();

  void SetEntityClass(const csClass *entityClass);
  const csClass *GetEntityClass() const;

  void AddProperty(csBPBaseProperty *property);
  void AddEntityState(csBPEntityState *entityState);

  csEntity *CreateEntity() const;

private:
  const csClass *m_entityClass;

  std::vector<csBPBaseProperty*> m_properties;
  std::vector<csBPEntityState*> m_entityStates;
};

