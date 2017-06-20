#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/entity/csrenderstate.hh>
#include <cobalt/csenums.hh>
#include <cobalt/physics/iphysicsstaticcollider.hh>
#include <cobalt/physics/csphysicsshapecontainer.hh>
#include <cobalt/entity/csstaticmeshstate.refl.hh>

class csMesh;
class csMaterial;
/**
* \ingroup entity
*/

CS_CLASS()
class CSE_API csStaticMeshState : public CS_SUPER(csRenderState)
{
  CS_CLASS_GEN;
public:
  csStaticMeshState();
  virtual ~csStaticMeshState();

  void SetMesh(csMesh *mesh);
  csMesh *GetMesh();
  const csMesh *GetMesh() const;

  csSize GetNumberOfMaterialsSlots() const;
  void SetMaterial(csMaterial *material, csSize slot = 0);
  csMaterial *GetMaterial(csSize slot = 0);
  const csMaterial *GetMaterial(csSize slot = 0) const;

  void SetColliderShape(csPhysicsShapeContainer *shapes);
  csPhysicsShapeContainer *GetColliderShape();
  const csPhysicsShapeContainer *GetColliderShape() const;
  void SetFriction(float friction);
  float GetFriction() const;
  void SetRestitution(float restitution);
  float GetRestitution() const;


  void SetCastShadow(bool castShadow);
  bool IsCastShadow() const;

  virtual void UpdateTransformation();


  virtual void Render(iGraphics *graphics, csRenderPass pass) const;

protected:
  virtual void OnAttachedToScene(csEntityScene *scene);
  virtual void OnDetachedFromScene(csEntityScene *scene);

  virtual void FillBoundingBox(csBoundingBox &bbox);
  virtual void PrivScan(csClipper *clipper, iGraphics *graphics, iEntityScan *entityScan, const csScanConfig &config);

private:

  void UpdateMaterialSlots();

  bool m_castShadow;

  csMesh* m_mesh;

  csSize m_numberOfMaterialSlots;
  csMaterial **m_materials;

  iPhysicsStaticCollider *m_staticCollider;
  csPhysicsShapeContainer *m_shapes;
  float m_friction;
  float m_restitution;
};


CS_FORCEINLINE csMesh *csStaticMeshState::GetMesh()
{
  return m_mesh;
}

CS_FORCEINLINE const csMesh *csStaticMeshState::GetMesh() const
{
  return m_mesh;
}

CS_FORCEINLINE csSize csStaticMeshState::GetNumberOfMaterialsSlots() const
{
  return m_numberOfMaterialSlots;
}

CS_FORCEINLINE csMaterial *csStaticMeshState::GetMaterial(csSize slot)
{
  if (slot >= m_numberOfMaterialSlots)
  {
    return 0;
  }
  return m_materials[slot];
}

CS_FORCEINLINE const csMaterial *csStaticMeshState::GetMaterial(csSize slot) const
{
  if (slot >= m_numberOfMaterialSlots)
  {
    return 0;
  }
  return m_materials[slot];
}

CS_FORCEINLINE void csStaticMeshState::SetCastShadow(bool castShadow)
{
  m_castShadow = castShadow;
}

CS_FORCEINLINE bool csStaticMeshState::IsCastShadow() const
{
  return m_castShadow;
}


CS_FORCEINLINE csPhysicsShapeContainer *csStaticMeshState::GetColliderShape()
{
  return m_shapes;
}

CS_FORCEINLINE const csPhysicsShapeContainer *csStaticMeshState::GetColliderShape() const
{
  return m_shapes;
}


CS_FORCEINLINE float csStaticMeshState::GetFriction() const
{
  return m_friction;
}

CS_FORCEINLINE float csStaticMeshState::GetRestitution() const
{
  return m_restitution;
}

