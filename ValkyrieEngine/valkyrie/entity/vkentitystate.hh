#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/vkdefs.hh>
#include <valkyrie/vktypes.hh>
#include <valkyrie/entity/vkentitystate.refl.hh>

class vkClipper;
class vkEntity;
class vkEntityScene;
class vkSpatialState;
struct IEntityScan;
struct IGraphics;

/**
* \ingroup entity
*/

VK_INTERFACE()
class VKE_API vkEntityState : public VK_SUPER(vkObject)
{
  VK_CLASS_GEN;

public:
  virtual ~vkEntityState();

  virtual void SetID(vkID id);
  vkID GetID() const;

  void SetName(const vkString &name);
  const vkString &GetName() const;

  virtual vkSpatialState *ToSpatialState();
  virtual const vkSpatialState *ToSpatialState() const;


  vkEntity *GetEntity();
  const vkEntity *GetEntity() const;

  /**
   * \name Live-cycles
   * @{
   */
  void Create();
  void AttachToEntity(vkEntity *entity);
  void Assemble();
  void AttachToScene(vkEntityScene *scene);
  void DetachFromScene(vkEntityScene *scene);
  void Disassemble();
  void DetachFromEntity(vkEntity *entity);
  void Destroy();
  /** 
   * @}
   */

  virtual void Update(float tpf);

protected:
  vkEntityState();

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
  virtual void OnAttachedToEntity(vkEntity *entity);

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
  virtual void OnAttachedToScene(vkEntityScene *scene);

  /**
   * \brief Called when the entity has been detached from the scene
   */
  virtual void OnDetachedFromScene(vkEntityScene *scene);
  
  /**
   * \brief Call when the entity was removed from scene but still all entity states are present
   *        with the entity.
   */
  virtual void OnDisassembled();

  /**
   * \brief Called when the state has been removed from the entity
   */
  virtual void OnDetachedFromEntity(vkEntity *entity);

  /**
   * \brief Called right before the entity object is removed from memory.
   */
  virtual void OnDestroyed();
  /**
  * @}
  */




private:
  vkID m_id;
  vkString m_name;

  bool m_created;
  bool m_assembled;
  vkEntity *m_entity;
  vkEntityScene *m_scene;
};

VK_FORCEINLINE void vkEntityState::SetID(vkID id)
{
  m_id = id;
}

VK_FORCEINLINE vkID vkEntityState::GetID() const
{
  return m_id;
}



VK_FORCEINLINE void vkEntityState::SetName(const vkString &name)
{
  m_name = name;
}

VK_FORCEINLINE const vkString &vkEntityState::GetName() const
{
  return m_name;
}

VK_FORCEINLINE vkEntity *vkEntityState::GetEntity()
{
  return m_entity;
}

VK_FORCEINLINE const vkEntity *vkEntityState::GetEntity() const
{
  return m_entity;
}

