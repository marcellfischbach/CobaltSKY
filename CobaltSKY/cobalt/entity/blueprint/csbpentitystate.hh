
#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>

#include <cobalt/entity/blueprint/csbpentitystate.refl.hh>

class csBPBaseProperty;
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

  void SetEntityStateClass(const csClass *entityStateClass);
  const csClass *GetEntityStateClass() const;
  void AddProperty(csBPBaseProperty *property);

  csEntityState *CreateEntityState() const;

private:

  unsigned m_id;
  unsigned m_parentId;
  bool m_root;

  const csClass *m_entityStateClass;
  std::vector<csBPBaseProperty*> m_properties;

};


