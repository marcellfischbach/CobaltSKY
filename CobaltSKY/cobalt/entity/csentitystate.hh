#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/csdefs.hh>
#include <cobalt/cstypes.hh>
#include <cobalt/entity/csentitystate.refl.hh>

class csClipper;
class csEntity;
class csEntityScene;
class csSpatialState;
struct iEntityScan;
struct iGraphics;

/**
* \ingroup entity
*/


CS_INTERFACE()
class CSE_API csEntityState : public CS_SUPER(csObject)
{
  CS_CLASS_GEN;

public:
  virtual ~csEntityState();

  virtual void SetID(csID id);
  csID GetID() const;

  void SetName(const std::string &name);
  const std::string &GetName() const;

  virtual csSpatialState *ToSpatialState();
  virtual const csSpatialState *ToSpatialState() const;


  csEntity *GetEntity();
  const csEntity *GetEntity() const;

  /**
   * \name Live-cycles
   * @{
   */
  void Create();
  void AttachToEntity(csEntity *entity);
  void Assemble();
  void AttachToScene(csEntityScene *scene);
  void DetachFromScene(csEntityScene *scene);
  void Disassemble();
  void DetachFromEntity(csEntity *entity);
  void Destroy();
  /** 
   * @}
   */

  virtual void Update(float tpf);

protected:
  csEntityState();

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
  virtual void OnAttachedToEntity(csEntity *entity);

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
  virtual void OnAttachedToScene(csEntityScene *scene);

  /**
   * \brief Called when the entity has been detached from the scene
   */
  virtual void OnDetachedFromScene(csEntityScene *scene);
  
  /**
   * \brief Call when the entity was removed from scene but still all entity states are present
   *        with the entity.
   */
  virtual void OnDisassembled();

  /**
   * \brief Called when the state has been removed from the entity
   */
  virtual void OnDetachedFromEntity(csEntity *entity);

  /**
   * \brief Called right before the entity object is removed from memory.
   */
  virtual void OnDestroyed();
  /**
  * @}
  */




private:
  csID m_id;

  CS_PROPERTY(Name)
  std::string m_name;

  bool m_created;
  bool m_assembled;
  csEntity *m_entity;
  csEntityScene *m_scene;
};

CS_FORCEINLINE void csEntityState::SetID(csID id)
{
  m_id = id;
}

CS_FORCEINLINE csID csEntityState::GetID() const
{
  return m_id;
}



CS_FORCEINLINE void csEntityState::SetName(const std::string &name)
{
  m_name = name;
}

CS_FORCEINLINE const std::string &csEntityState::GetName() const
{
  return m_name;
}

CS_FORCEINLINE csEntity *csEntityState::GetEntity()
{
  return m_entity;
}

CS_FORCEINLINE const csEntity *csEntityState::GetEntity() const
{
  return m_entity;
}

