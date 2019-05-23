#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/core/csresourcewrapper.hh>
#include <cobalt/csdefs.hh>
#include <cobalt/cstypes.hh>
#include <cobalt/entity/csentitystate.refl.hh>


/**
* \ingroup entity
*/

namespace cs
{
struct iEntityScan;
struct iGraphics;
class Clipper;
class Entity;
class EntityScene;
class SpatialState;


CS_CLASS()
class CSE_API EntityState : public CS_SUPER(cs::Object)
{
  CS_CLASS_GEN;

public:
  virtual ~EntityState();

  virtual void SetId(csID id);
  csID GetId() const;

  void SetName(const std::string & name);
  const std::string& GetName() const;

  virtual cs::SpatialState* ToSpatialState();
  virtual const cs::SpatialState* ToSpatialState() const;

  cs::Entity* GetEntity();
  const cs::Entity* GetEntity() const;

  /**
   * \name Live-cycles
   * @{
   */
  void Create();
  void AttachToEntity(cs::Entity * entity);
  void Assemble();
  void AttachToScene(cs::EntityScene * scene);
  void DetachFromScene(cs::EntityScene * scene);
  void Disassemble();
  void DetachFromEntity(cs::Entity * entity);
  void Destroy();
  /**
   * @}
   */

  virtual void Update(float tpf);

protected:
  EntityState();

  /**
  * \name Live-cycle event listeners
  * @{
  */

  /**
   * \brief Called after almost all parameter have been set during loading.
   */
  virtual void OnCreated();

  /**
   * \brief Called when the entitye state is attached to the entity it belongs to.
   */
  virtual void OnAttachedToEntity(cs::Entity * entity);

  /**
   * \brief Called after all state have been attached to the same entity this state is attached to.
   *
   * If a state need access to other entity state within the same entity, those connections should
   * be done here.
   *
   * This is usually called before the entity is attached to a scene
   */
  virtual void OnAssembled();

  /**
   * \brief Called when the entity has been attached to the scene.
   */
  virtual void OnAttachedToScene(cs::EntityScene * scene);

  /**
   * \brief Called when the entity has been detached from the scene
   */
  virtual void OnDetachedFromScene(cs::EntityScene * scene);

  /**
   * \brief Call when the entity was removed from scene but still all entity states are present
   *        with the entity.
   */
  virtual void OnDisassembled();

  /**
   * \brief Called when the state has been removed from the entity
   */
  virtual void OnDetachedFromEntity(cs::Entity * entity);

  /**
   * \brief Called right before the entity object is removed from memory.
   */
  virtual void OnDestroyed();
  /**
  * @}
  */




private:
  csID m_id;

  CS_PROPERTY(name = Name)
    std::string m_name;

  bool m_created;
  bool m_assembled;
  cs::Entity * m_entity;
  cs::EntityScene * m_scene;
};


CS_CLASS()
class CSE_API EntityStateWrapper : public CS_SUPER(cs::ResourceWrapper)
{
  CS_CLASS_GEN;
  CS_RESOURCE_WRAPPER(cs::EntityState, EntityStateWrapper, cs::ResourceWrapper);
};


}

CS_FORCEINLINE csID cs::EntityState::GetId() const
{
  return m_id;
}



CS_FORCEINLINE void cs::EntityState::SetName(const std::string &name)
{
  m_name = name;
}

CS_FORCEINLINE const std::string &cs::EntityState::GetName() const
{
  return m_name;
}

CS_FORCEINLINE cs::Entity *cs::EntityState::GetEntity()
{
  return m_entity;
}

CS_FORCEINLINE const cs::Entity *cs::EntityState::GetEntity() const
{
  return m_entity;
}

