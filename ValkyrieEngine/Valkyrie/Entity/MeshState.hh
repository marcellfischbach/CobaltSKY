#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Entity/RenderState.hh>
#include <Valkyrie/Enums.hh>
#include <Valkyrie/Entity/MeshState.refl.hh>
#include <Valkyrie/Physics/IPhysicsCollider.hh>
#include <Valkyrie/Physics/PhysicsShapeContainer.hh>

class vkMesh;
class vkMaterialInstance;
/**
* \ingroup entity
*/

VK_CLASS()
class VKE_API vkStaticMeshState : public vkRenderState
{
  VK_CLASS_GEN;
public:
  vkStaticMeshState();
  virtual ~vkStaticMeshState();

  void SetMesh(vkMesh *mesh);
  vkMesh *GetMesh();
  const vkMesh *GetMesh() const;

  vkSize GetNumberOfMaterialsSlots() const;
  void SetMaterial(vkMaterialInstance *material, vkSize slot = 0);
  vkMaterialInstance *GetMaterial(vkSize slot = 0);
  const vkMaterialInstance *GetMaterial(vkSize slot = 0) const;

  void SetColliderShape(vkPhysicsShapeContainer *shapes);
  vkPhysicsShapeContainer *GetColliderShape();
  const vkPhysicsShapeContainer *GetColliderShape() const;
  void SetFriction(float friction);
  float GetFriction() const;
  void SetRestitution(float restitution);
  float GetRestitution() const;


  void SetCastShadow(bool castShadow);
  bool IsCastShadow() const;

  virtual void UpdateTransformation();


  virtual void Render(IGraphics *graphics, vkRenderPass pass) const;

protected:
  virtual void OnAttachedToScene(vkEntityScene *scene);
  virtual void OnDetachedFromScene(vkEntityScene *scene);

  virtual void FillBoundingBox(vkBoundingBox &bbox);
  virtual void PrivScan(vkClipper *clipper, IGraphics *graphics, IEntityScan *entityScan, const vkScanConfig &config);

private:

  void UpdateMaterialSlots();

  bool m_castShadow;

  vkMesh* m_mesh;

  vkSize m_numberOfMaterialSlots;
  vkMaterialInstance **m_materials;

  IPhysicsStaticCollider *m_staticCollider;
  vkPhysicsShapeContainer *m_shapes;
  float m_friction;
  float m_restitution;
};

VK_FORCEINLINE vkMesh *vkStaticMeshState::GetMesh()
{
  return m_mesh;
}

VK_FORCEINLINE const vkMesh *vkStaticMeshState::GetMesh() const
{
  return m_mesh;
}

VK_FORCEINLINE vkSize vkStaticMeshState::GetNumberOfMaterialsSlots() const
{
  return m_numberOfMaterialSlots;
}

VK_FORCEINLINE vkMaterialInstance *vkStaticMeshState::GetMaterial(vkSize slot)
{
  if (slot >= m_numberOfMaterialSlots)
  {
    return 0;
  }
  return m_materials[slot];
}

VK_FORCEINLINE const vkMaterialInstance *vkStaticMeshState::GetMaterial(vkSize slot) const
{
  if (slot >= m_numberOfMaterialSlots)
  {
    return 0;
  }
  return m_materials[slot];
}

VK_FORCEINLINE void vkStaticMeshState::SetCastShadow(bool castShadow)
{
  m_castShadow = castShadow;
}

VK_FORCEINLINE bool vkStaticMeshState::IsCastShadow() const
{
  return m_castShadow;
}


VK_FORCEINLINE vkPhysicsShapeContainer *vkStaticMeshState::GetColliderShape()
{
  return m_shapes;
}

VK_FORCEINLINE const vkPhysicsShapeContainer *vkStaticMeshState::GetColliderShape() const
{
  return m_shapes;
}


VK_FORCEINLINE float vkStaticMeshState::GetFriction() const
{
  return m_friction;
}

VK_FORCEINLINE float vkStaticMeshState::GetRestitution() const
{
  return m_restitution;
}