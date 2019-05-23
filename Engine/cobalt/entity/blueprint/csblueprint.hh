#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/core/csresourcewrapper.hh>

#include <cobalt/entity/blueprint/csblueprint.refl.hh>



namespace cs
{
class BPEntity;
class BPEntityState;
class EntityWrapper;
class EntityStateWrapper;


CS_CLASS()
class CSE_API Blueprint : public cs::iObject
{
  CS_CLASS_GEN_OBJECT;
public:
  Blueprint();
  virtual ~Blueprint();

  cs::EntityWrapper* CreateEntity() const;
  cs::EntityStateWrapper* CreateEntityState() const;

  void SetEntity(cs::BPEntity* entity);
  void SetEntityState(cs::BPEntityState* entityState);

private:

  cs::BPEntity* m_entity;
  cs::BPEntityState* m_entityState;
};

CS_CLASS()
class BlueprintWrapper : public CS_SUPER(cs::ResourceWrapper)
{
  CS_CLASS_GEN;
  CS_RESOURCE_WRAPPER(cs::Blueprint, BlueprintWrapper, cs::ResourceWrapper);
};

}