
#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>

#include <vector>

#include <cobalt/entity/blueprint/csbpentity.refl.hh>

class csBPEntityState;
class csBPProperty;
class csEntity;
class csEntityState;


class CSE_API csBPEntity 
{
public:
  csBPEntity();

  void SetEntityClass(const csClass *entityClass);
  const csClass *GetEntityClass() const;

  void AddProperty(csBPProperty *property);

  csEntity *CreateEntity() const;

private:
  const csClass *m_entityClass;

  std::vector<csBPProperty*> m_properties;
};

