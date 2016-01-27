#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Defs.hh>
#include <vector>
#include <Valkyrie/Entity/Entity.refl.hh>

class vkBoundingBox;
class vkClipper;
class vkEntityScene;
class vkEntityState;
class vkSpatialState;
class vkTransformation;

struct vkScanConfig;
struct IEntityScan;
struct IGraphics;
struct IPhysicsBody;

/**
 * \ingroup entity
 */
VK_CLASS()
class VKE_API vkEntity : public vkObject
{
  VK_CLASS_GEN;
public:
  vkEntity();
  virtual ~vkEntity();

  vkID GetID() const;

  void SetScene(vkEntityScene *scene);
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

  const vkBoundingBox &GetBoundingBox() const;
  vkTransformation GetTransformation();
  void FinishTransformation();
  void UpdateBoundingBox();

  static vkID GetNextID();

  virtual void Scan(vkClipper *clipper, IGraphics *graphics, IEntityScan *entityScan, const vkScanConfig &config);

  void AttachEntity(vkEntity *entity, vkSpatialState *parentState = 0);
  void AttachEntity(vkEntity *entity, const vkString &parentStateName);
  void DetachEntity(vkEntity *entity);


  void SetClippingRange(float min = -FLT_MAX, float max = FLT_MAX);

protected:
  virtual void UpdateScene(vkEntityScene *scene) { }
  void PerformTransformation();

private:
  vkID m_id;
  vkString m_name;

  vkSpatialState *m_rootState;
  std::vector<vkEntityState*> m_states;

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

