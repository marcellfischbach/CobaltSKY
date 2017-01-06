#pragma once


#include <valkyrie/vkexport.hh>
#include <valkyrie/vkenums.hh>
#include <valkyrie/graphics/scene/vkspatialnode.hh>
#include <valkyrie/graphics/scene/vkgeometrynode.refl.hh>

struct IGraphics;
class vkMaterialInstance;
class vkMesh;

VK_CLASS()
class VKE_API vkGeometryNode : public VK_SUPER(vkSpatialNode)
{
  VK_CLASS_GEN;
public:
  vkGeometryNode(vkMesh *mesh = 0, vkMaterialInstance *material = 0);
  virtual ~vkGeometryNode();


  void SetMesh(vkMesh *mesh);
  vkMesh *GetMesh();
  const vkMesh *GetMesh() const;

  void SetMaterial(vkMaterialInstance *material);
  vkMaterialInstance *GetMaterial();
  const vkMaterialInstance *GetMaterial() const;

  void Render(IGraphics *renderer, vkRenderPass renderPass, vkUInt8 lod = 0);

protected:
  virtual void PrivScan(const vkClipper *clipper, IGraphics *renderer, IScanCallback *callback);

  virtual void UpdateBoundingBox(vkBoundingBox &bbox);

private:
  vkMesh *m_mesh;
  vkMaterialInstance *m_material;

};


VK_FORCEINLINE vkMesh *vkGeometryNode::GetMesh()
{
  return m_mesh;
}

VK_FORCEINLINE const vkMesh *vkGeometryNode::GetMesh() const
{
  return m_mesh;
}


VK_FORCEINLINE vkMaterialInstance *vkGeometryNode::GetMaterial()
{
  return m_material;
}

VK_FORCEINLINE const vkMaterialInstance *vkGeometryNode::GetMaterial() const
{
  return m_material;
}
