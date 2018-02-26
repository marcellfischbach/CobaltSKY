#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/entity/csrenderstate.hh>
#include <cobalt/csenums.hh>
#include <cobalt/physics/csphysicsshapecontainer.hh>
#include <cobalt/entity/csstaticmeshstate.refl.hh>

#include <vector>

struct iMesh;
struct iPhysicsBaseCollider;
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

  CS_FUNCTION()
  void SetMesh(iMesh *mesh);
  CS_FUNCTION()
  iMesh *GetMesh();
  CS_FUNCTION()
  const iMesh *GetMesh() const;

  CS_FUNCTION()
  csSize GetNumberOfMaterials() const;
  CS_FUNCTION()
  void SetMaterial(csSize idx, csMaterial *material);
  CS_FUNCTION()
  csMaterial *GetMaterial(csSize idx = 0);
  CS_FUNCTION()
  const csMaterial *GetMaterial(csSize idx = 0) const;

  CS_FUNCTION()
  csSize GetNumberOfColliderShapes () const;
  CS_FUNCTION()
  void AddColliderShape(iPhysicsShape *shape);
  CS_FUNCTION()
  void SetColliderShape(csSize idx, iPhysicsShape *shape);
  CS_FUNCTION()
  iPhysicsShape *GetColliderShape (csSize idx = 0);
  CS_FUNCTION()
  const iPhysicsShape *GetColliderShape (csSize idx = 0) const;

  CS_FUNCTION()
  void SetFriction(float friction);
  CS_FUNCTION()
  float GetFriction() const;
  CS_FUNCTION()
  void SetRestitution(float restitution);
  CS_FUNCTION()
  float GetRestitution() const;


  CS_FUNCTION()
  void SetCastShadow(bool castShadow);
  CS_FUNCTION()
  bool IsCastShadow() const;

  virtual void UpdateTransformation();



  virtual void Render(iGraphics *graphics, csRenderPass pass) const;

  virtual unsigned GetNumberOfRenderCalls() const;
  virtual unsigned GetNumberOfTotalTrigons() const;

protected:
  virtual void OnAttachedToScene(csEntityScene *scene);
  virtual void OnDetachedFromScene(csEntityScene *scene);

  virtual void FillBoundingBox(csBoundingBox &bbox);
  virtual void PrivScan(csClipper *clipper, iGraphics *graphics, iEntityScan *entityScan, const csScanConfig &config);

private:

  iPhysicsBaseCollider *GetCollider();

  void UpdateMaterialSlots();

  bool m_castShadow;

  CS_PROPERTY(name = Mesh)
  iMesh* m_mesh;

  CS_PROPERTY(name = Materials, type= csMaterial)
  std::vector<csMaterial*> m_materials;
  CS_PROPERTY(name = ColliderShapes, type= iPhysicsShape)
  std::vector<iPhysicsShape*> m_colliderShapes;

  bool m_enableCollision;
  csPhysicsColliderType m_colliderType;
  iPhysicsBaseCollider *m_collider;


  CS_PROPERTY(name=Friction)
  float m_friction;
  CS_PROPERTY(name=Restitution)
  float m_restitution;

  CS_PROPERTY(name=AutoInertia)
  bool m_autoInertia;
  CS_PROPERTY(name=Inertia)
  csVector3f m_inertia;
  CS_PROPERTY(name=Mass)
  float m_mass;
};


CS_FORCEINLINE iMesh *csStaticMeshState::GetMesh()
{
  return m_mesh;
}

CS_FORCEINLINE const iMesh *csStaticMeshState::GetMesh() const
{
  return m_mesh;
}

CS_FORCEINLINE csSize csStaticMeshState::GetNumberOfMaterials() const
{
  return m_materials.size();
}

CS_FORCEINLINE csMaterial *csStaticMeshState::GetMaterial(csSize idx)
{
  if (idx >= m_materials.size())
  {
    return 0;
  }
  return m_materials[idx];
}

CS_FORCEINLINE const csMaterial *csStaticMeshState::GetMaterial(csSize idx) const
{
  if (idx >= m_materials.size())
  {
    return 0;
  }
  return m_materials[idx];
}


CS_FORCEINLINE csSize csStaticMeshState::GetNumberOfColliderShapes() const
{
  return m_colliderShapes.size();
}

CS_FORCEINLINE iPhysicsShape *csStaticMeshState::GetColliderShape(csSize idx)
{
  if (idx >= m_colliderShapes.size())
  {
    return 0;
  }
  return m_colliderShapes[idx];
}

CS_FORCEINLINE const iPhysicsShape *csStaticMeshState::GetColliderShape(csSize idx) const
{
  if (idx >= m_colliderShapes.size())
  {
    return 0;
  }
  return m_colliderShapes[idx];
}


CS_FORCEINLINE void csStaticMeshState::SetCastShadow(bool castShadow)
{
  m_castShadow = castShadow;
}

CS_FORCEINLINE bool csStaticMeshState::IsCastShadow() const
{
  return m_castShadow;
}

CS_FORCEINLINE float csStaticMeshState::GetFriction() const
{
  return m_friction;
}

CS_FORCEINLINE float csStaticMeshState::GetRestitution() const
{
  return m_restitution;
}

