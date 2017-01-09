#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/vkdefs.hh>
#include <vector>

#include <valkyrie/entity/vkentity.refl.hh>

class vkBoundingBox;
class vkClipper;
class vkEntityScene;
class vkEntityState;
class vkSpatialState;
class vkTransformation;

struct vkScanConfig;
struct iEntityScan;
struct iGraphics;
struct iPhysicsBody;

/**
 * \ingroup entity
 */
VK_CLASS()
class VKE_API vkEntity : public VK_SUPER(vkObject)
{
  VK_CLASS_GEN;
public:
  vkEntity();
  virtual ~vkEntity();

  vkID GetID() const;

  vkEntityScene *GetScene();
  const vkEntityScene *GetScene() const;

  void SetName(const vkString &name);
  const vkString &GetName() const;

  vkEntityState *GetState(vkID id);
  const vkEntityState *GetState(vkID id) const;
  vkEntityState *GetState(const vkString &name);
  const vkEntityState *GetState(const vkString &name) const;



  void SetRootState(vkSpatialState *rootState);
  void AddState(vkEntityState *state);
  void AddState(vkSpatialState *state, vkSpatialState *parentState);
  vkSpatialState *GetRootState();
  const vkSpatialState *GetRootState() const;

  vkEntity *GetParentEntity() ;
  const vkEntity *GetParentEntity() const;

  const vkBoundingBox &GetBoundingBox() const;
  vkTransformation GetTransformation();
  void FinishTransformation();
  void UpdateBoundingBox();

  static vkID GetNextID();

  virtual void Scan(vkClipper *clipper, iGraphics *graphics, iEntityScan *entityScan, const vkScanConfig &config);

  void AttachEntity(vkEntity *entity, vkSpatialState *parentState = 0);
  void AttachEntity(vkEntity *entity, const vkString &parentStateName);
  void DetachEntity(vkEntity *entity);


  void SetClippingRange(float min = -FLT_MAX, float max = FLT_MAX);

  virtual void Update(float tpf);

  /**
  * \name Live-cycles
  * @{
  */
  void Create();
  void Assemble();
  void AttachToEntity(vkEntity *entity);
  void AttachToScene(vkEntityScene *scene);
  void DetachFromScene(vkEntityScene *scene);
  void DetachFromEntity(vkEntity *entity);
  void Disassemble();
  void Destroy();
  /**
  * @}
  */
protected:
  virtual void UpdateScene(vkEntityScene *scene) { }
  void PerformTransformation();

  /**
  * \name Live-cycle event listeners
  * @{
  */

  virtual void OnCreated();
  virtual void OnAssembled();
  virtual void OnAttachedToEntity(vkEntity *entity);
  virtual void OnAttachedToScene(vkEntityScene *scene);
  virtual void OnDetachedFromScene(vkEntityScene *scene);
  virtual void OnDetachedFromEntity(vkEntity *entity);
  virtual void OnDisassembled();
  virtual void OnDestroyed();
  /**
  * @}
  */

private:
  vkID m_id;
  vkString m_name;

  vkSpatialState *m_rootState;
  std::vector<vkEntityState*> m_states;

  bool m_created;
  bool m_assemabled;
  vkEntityScene *m_scene;
  vkEntity *m_parentEntity;
  std::vector<vkEntity*> m_children;
};

VK_FORCEINLINE vkID vkEntity::GetID() const
{
  return m_id;
}

VK_FORCEINLINE vkEntityScene *vkEntity::GetScene()
{
  return m_scene;
}

VK_FORCEINLINE const vkEntityScene *vkEntity::GetScene() const
{
  return m_scene;
}


VK_FORCEINLINE void vkEntity::SetName(const vkString &name)
{
  m_name = name;
}

VK_FORCEINLINE const vkString &vkEntity::GetName() const
{
  return m_name;
}


VK_FORCEINLINE vkSpatialState *vkEntity::GetRootState()
{
  return m_rootState;
}

VK_FORCEINLINE const vkSpatialState *vkEntity::GetRootState() const
{
  return m_rootState;
}


VK_FORCEINLINE vkEntity *vkEntity::GetParentEntity()
{
  return m_parentEntity;
}

VK_FORCEINLINE const vkEntity *vkEntity::GetParentEntity() const
{
  return m_parentEntity;
}


