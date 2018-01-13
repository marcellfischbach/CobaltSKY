
#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>

#include <cobalt/entity/blueprint/csbpentitystate.refl.hh>

class csEntityState;

CS_CLASS()
class CSE_API csBPEntityState : public iObject
{
  CS_CLASS_GEN_OBJECT;
public:
  csBPEntityState();

  void SetEntityStateClass(const csClass *entityStateClass);

  csEntityState *CreateEntityState() const;

private:

  const csClass *m_entityStateClass;

};


