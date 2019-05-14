
#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>

class csPropertySetter;
class csEntity;
class csEntityState;

class CSE_API csBPEntityState 
{
public:
  csBPEntityState();
  ~csBPEntityState();

  void SetId(unsigned id);
  unsigned GetId() const;

  void SetParentId(unsigned id);
  unsigned GetParentId() const;

  void SetRoot(bool root);
  bool IsRoot() const;

  void SetEntityStateClass(const cs::Class *entityStateClass);
  const cs::Class *GetEntityStateClass() const;
  void AddProperty(csPropertySetter *property);

  csEntityState *CreateEntityState() const;

private:

  unsigned m_id;
  unsigned m_parentId;
  bool m_root;

  const cs::Class *m_entityStateClass;
  std::vector<csPropertySetter*> m_properties;

};


