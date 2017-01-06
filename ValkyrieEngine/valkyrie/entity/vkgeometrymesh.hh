#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/entity/vkgeometrydata.hh>

#include <valkyrie/entity/vkgeometrymesh.refl.hh>

/**
* \ingroup entity
*/

VK_CLASS()
class VKE_API vkGeometryMesh : public VK_SUPER(vkGeometryData)
{
  VK_CLASS_GEN;

public:
  vkGeometryMesh();
  virtual ~vkGeometryMesh();

  void SetMesh(vkMesh *mesh);
  vkMesh *GetMesh();
  const vkMesh *GetMesh() const;

  void SetMaterial(vkMultiMaterial *material);
  vkMultiMaterial *GetMaterial();
  const vkMultiMaterial *GetMaterial() const;

  void SetLocalTransorm(const vkMatrix4f &localTransform);
  const vkMatrix4f &GetLocalTransform() const;

  const vkMatrix4f &GetGlobalTransform() const;

  void UpdateTransformation(const vkMatrix4f &parentTransform);

  virtual void Scan(vkClipper *clipper, IGraphics *graphics, IEntityScan *entityScan, const vkScanConfig &config);

  virtual vkGeometryData *Clone();

  void Render(IGraphics *graphics, vkRenderPass pass, vkUInt8 lod = 0);
private:
  vkMatrix4f m_localTransform;
  vkMatrix4f m_globalTransform;

  vkMesh *m_mesh;
  vkMultiMaterial *m_material;

};



VK_FORCEINLINE vkMesh *vkGeometryMesh::GetMesh()
{
  return m_mesh;
}

VK_FORCEINLINE const vkMesh *vkGeometryMesh::GetMesh() const
{
  return m_mesh;
}


VK_FORCEINLINE vkMultiMaterial *vkGeometryMesh::GetMaterial()
{
  return m_material;
}

VK_FORCEINLINE const vkMultiMaterial *vkGeometryMesh::GetMaterial() const
{
  return m_material;
}


VK_FORCEINLINE const vkMatrix4f &vkGeometryMesh::GetGlobalTransform() const
{
  return m_globalTransform;
}


