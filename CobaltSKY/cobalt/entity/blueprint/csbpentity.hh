
#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>

#include <vector>

#include <cobalt/entity/blueprint/csbpentity.refl.hh>

class csBPEntityState;
class csPropertySetter;
class csEntity;
class csEntityState;


class CSE_API csBPEntity 
{
public:
  csBPEntity();
  ~csBPEntity();

  void SetEntityClass(const csClass *entityClass);
  const csClass *GetEntityClass() const;

  void AddProperty(csPropertySetter *property);
  void AddEntityState(csBPEntityState *entityState);

  csEntity *CreateEntity() const;

  csBPEntityState *GetEntityStateByID(unsigned id) const;

private:
  const csClass *m_entityClass;

  std::vector<csPropertySetter*> m_properties;
  std::vector<csBPEntityState*> m_entityStates;
};

