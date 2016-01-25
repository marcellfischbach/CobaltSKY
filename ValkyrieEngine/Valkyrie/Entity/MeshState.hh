#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Entity/RenderState.hh>
#include <Valkyrie/Enums.hh>
#include <Valkyrie/Entity/MeshState.refl.hh>

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

  vkUInt32 GetNumberOfMaterialsSlots() const;
  void SetMaterial(vkMaterialInstance *material, vkUInt32 slot = 0);
  vkMaterialInstance *GetMaterial(vkUInt32 slot = 0);
  const vkMaterialInstance *GetMaterial(vkUInt32 slot = 0) const;

  void SetCastShadow(bool castShadow);
  bool IsCastShadow() const;

  virtual void Render(IGraphics *graphics, vkRenderPass pass) const;

protected:
  virtual void FillBoundingBox(vkBoundingBox &bbox);
  virtual void PrivScan(vkClipper *clipper, IGraphics *graphics, IEntityScan *entityScan, const vkScanConfig &config);

private:

  void UpdateMaterialSlots();

  bool m_castShadow;

  vkMesh* m_mesh;

  vkUInt32 m_numberOfMaterialSlots;
  vkMaterialInstance **m_materials;

};

VK_FORCEINLINE vkMesh *vkStaticMeshState::GetMesh()
{
  return m_mesh;
}

VK_FORCEINLINE const vkMesh *vkStaticMeshState::GetMesh() const
{
  return m_mesh;
}

VK_FORCEINLINE vkUInt32 vkStaticMeshState::GetNumberOfMaterialsSlots() const
{
  return m_numberOfMaterialSlots;
}

VK_FORCEINLINE vkMaterialInstance *vkStaticMeshState::GetMaterial(vkUInt32 slot)
{
  if (slot >= m_numberOfMaterialSlots)
  {
    return 0;
  }
  return m_materials[slot];
}

VK_FORCEINLINE const vkMaterialInstance *vkStaticMeshState::GetMaterial(vkUInt32 slot) const
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
