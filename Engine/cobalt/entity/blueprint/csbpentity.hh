
#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>

#include <vector>


class csBPEntityState;
class csPropertySetter;
class csEntity;
class csEntityState;


class CSE_API csBPEntity 
{
public:
  csBPEntity();
  ~csBPEntity();

  void SetEntityClass(const cs::Class *entityClass);
  const cs::Class *GetEntityClass() const;

  void AddProperty(csPropertySetter *property);
  void AddEntityState(csBPEntityState *entityState);

  csEntity *CreateEntity() const;

  csBPEntityState *GetEntityStateByID(unsigned id) const;

private:
  const cs::Class *m_entityClass;

  std::vector<csPropertySetter*> m_properties;
  std::vector<csBPEntityState*> m_entityStates;
};

