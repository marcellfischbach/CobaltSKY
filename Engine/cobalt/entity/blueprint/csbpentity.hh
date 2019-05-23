
#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>

#include <vector>


namespace cs
{
class BPEntityState;
class PropertySetter;


class Entity;
class EntityState;

class CSE_API BPEntity
{
public:
  BPEntity();
  ~BPEntity();

  void SetEntityClass(const cs::Class* entityClass);
  const cs::Class* GetEntityClass() const;

  void AddProperty(cs::PropertySetter* property);
  void AddEntityState(cs::BPEntityState* entityState);

  cs::Entity* CreateEntity() const;

  cs::BPEntityState* GetEntityStateByID(unsigned id) const;

private:
  const cs::Class* m_entityClass;

  std::vector<cs::PropertySetter*> m_properties;
  std::vector<cs::BPEntityState*> m_entityStates;
};


}

