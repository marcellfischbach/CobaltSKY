#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Enums.hh>
#include <Valkyrie/Graphics/Scene/SpatialNode.hh>
#include <Valkyrie/Graphics/Scene/GeometryNode.refl.hh>

struct IRenderer;
class vkMaterialInstance;
class vkMesh;

VK_CLASS();
class VKE_API vkGeometryNode : public vkSpatialNode
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

  void Render(IRenderer *renderer, vkRenderPass renderPass, vkUInt8 lod = 0);

protected:
  virtual void PrivScan(const vkClipper *clipper, IRenderer *renderer, IScanCallback *callback);

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

