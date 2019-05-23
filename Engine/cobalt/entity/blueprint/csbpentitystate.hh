
#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>

namespace cs
{
class PropertySetter;
class Entity;
class EntityState;

class CSE_API BPEntityState
{
public:
  BPEntityState();
  ~BPEntityState();

  void SetId(unsigned id);
  unsigned GetId() const;

  void SetParentId(unsigned id);
  unsigned GetParentId() const;

  void SetRoot(bool root);
  bool IsRoot() const;

  void SetEntityStateClass(const cs::Class* entityStateClass);
  const cs::Class* GetEntityStateClass() const;
  void AddProperty(cs::PropertySetter* property);

  cs::EntityState* CreateEntityState() const;

private:

  unsigned m_id;
  unsigned m_parentId;
  bool m_root;

  const cs::Class* m_entityStateClass;
  std::vector<cs::PropertySetter*> m_properties;

};



}

