#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/entity/csrenderstate.hh>
#include <cobalt/csenums.hh>
#include <cobalt/physics/csphysicsshape.hh>
#include <cobalt/entity/csstaticmeshstate.refl.hh>

#include <vector>


namespace cs
{
struct iMesh;
struct iPhysicsBaseCollider;
class Material;

/**
* \ingroup entity
*/

CS_CLASS()
class CSE_API StaticMeshState : public CS_SUPER(cs::RenderState)
{
  CS_CLASS_GEN;
public:
  StaticMeshState();
  virtual ~StaticMeshState();

  CS_FUNCTION()
    void SetMesh(cs::iMesh * mesh);
  CS_FUNCTION()
    cs::iMesh* GetMesh();
  CS_FUNCTION()
    const cs::iMesh* GetMesh() const;

  CS_FUNCTION()
    csSize GetNumberOfMaterials() const;
  CS_FUNCTION()
    void SetMaterial(csSize idx, cs::Material * material);
  CS_FUNCTION()
    cs::Material* GetMaterial(csSize idx = 0);
  CS_FUNCTION()
    const cs::Material * GetMaterial(csSize idx = 0) const;

  CS_FUNCTION()
    cs::PhysicsShape * GetColliderShape();
  CS_FUNCTION()
    const cs::PhysicsShape * GetColliderShape() const;
  CS_FUNCTION()
    void SetColliderShape(cs::PhysicsShape * colliderShape);

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



  virtual void Render(cs::iGraphics * graphics, cs::eRenderPass pass) const;

  virtual size_t GetNumberOfRenderCalls() const;
  virtual size_t GetNumberOfTotalTrigons() const;

protected:
  virtual void OnAttachedToScene(cs::EntityScene * scene);
  virtual void OnDetachedFromScene(cs::EntityScene * scene);

  virtual void FillBoundingBox(cs::BoundingBox & bbox);
  virtual void PrivScan(cs::Clipper * clipper, cs::iGraphics * graphics, cs::iEntityScan * entityScan, const cs::ScanConfig & config);

private:

  cs::iPhysicsBaseCollider * GetCollider();

  void UpdateMaterialSlots();

  bool m_castShadow;

  CS_PROPERTY()
    cs::iMesh * m_mesh;

  CS_PROPERTY()
    std::vector<cs::Material*> m_materials;

  CS_PROPERTY()
    cs::PhysicsShape * m_colliderShape;

  bool m_enableCollision;
  cs::ePhysicsColliderType m_colliderType;

  CS_PROPERTY()
    cs::iPhysicsBaseCollider * m_collider;

  CS_PROPERTY()
    float m_friction;
  CS_PROPERTY()
    float m_restitution;

  CS_PROPERTY()
    bool m_autoInertia;
  CS_PROPERTY()
    cs::Vector3f m_inertia;
  CS_PROPERTY()
    float m_mass;
};

}


CS_FORCEINLINE cs::iMesh *cs::StaticMeshState::GetMesh()
{
  return m_mesh;
}

CS_FORCEINLINE const cs::iMesh *cs::StaticMeshState::GetMesh() const
{
  return m_mesh;
}

CS_FORCEINLINE csSize cs::StaticMeshState::GetNumberOfMaterials() const
{
  return m_materials.size();
}

CS_FORCEINLINE cs::Material *cs::StaticMeshState::GetMaterial(csSize idx)
{
  if (idx >= m_materials.size())
  {
    return 0;
  }
  return m_materials[idx];
}

CS_FORCEINLINE const cs::Material *cs::StaticMeshState::GetMaterial(csSize idx) const
{
  if (idx >= m_materials.size())
  {
    return 0;
  }
  return m_materials[idx];
}

CS_FORCEINLINE cs::PhysicsShape *cs::StaticMeshState::GetColliderShape()
{
  return m_colliderShape;
}

CS_FORCEINLINE const cs::PhysicsShape *cs::StaticMeshState::GetColliderShape() const
{
  return m_colliderShape;
}

CS_FORCEINLINE void cs::StaticMeshState::SetCastShadow(bool castShadow)
{
  m_castShadow = castShadow;
}

CS_FORCEINLINE bool cs::StaticMeshState::IsCastShadow() const
{
  return m_castShadow;
}

CS_FORCEINLINE float cs::StaticMeshState::GetFriction() const
{
  return m_friction;
}

CS_FORCEINLINE float cs::StaticMeshState::GetRestitution() const
{
  return m_restitution;
}

