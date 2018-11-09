#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/csresourcewrapper.hh>

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

CS_CLASS()
class csBlueprintWrapper : public CS_SUPER(csResourceWrapper)
{
  CS_CLASS_GEN;
  CS_RESOURCE_WRAPPER(csBlueprint, csBlueprintWrapper, csResourceWrapper);
};

//public:
//  csBlueprintWrapper (csBlueprint *blueprint = nullptr)
//    : csResourceWrapper(blueprint)
//    , m_blueprint(nullptr)
//  {
//    CS_SET(m_blueprint, blueprint);
//  }

//  virtual void Set(iObject *object)
//  {
//    csBlueprint *blueprint = csQueryClass<csBlueprint>(object);
//    csResourceWrapper::Set(blueprint);
//    CS_SET(m_blueprint, blueprint);
//  }

//  CS_FORCEINLINE csBlueprint *Get()
//  {
//    return m_blueprint;
//  }

//private:
//  csBlueprint *m_blueprint;
//};

