

#include <Valkyrie/Graphics/Scene/GeometryNode.hh>
#include <Valkyrie/Graphics/IRenderer.hh>
#include <Valkyrie/Graphics/Material.hh>
#include <Valkyrie/Graphics/Mesh.hh>


vkGeometryNode::vkGeometryNode(vkMesh *mesh, vkMaterialInstance *material)
  : vkSpatialNode()
  , m_mesh(0)
  , m_material(0)
{
  SetMesh(mesh);
  SetMaterial(material);
}

vkGeometryNode::~vkGeometryNode()
{

}


void vkGeometryNode::SetMesh(vkMesh* mesh)
{
  VK_SET(m_mesh, mesh);
}


void vkGeometryNode::SetMaterial(vkMaterialInstance *material)
{
  VK_SET(m_material, material);
}


void vkGeometryNode::Render(IRenderer *renderer, vkRenderPass pass, vkUInt8 lod)
{
  if (!m_material || !m_mesh)
  {
    return;
  }

  if (m_material->Bind(renderer, pass))
  {
    renderer->SetModelMatrix(GetMatrix());
    m_mesh->Render(renderer, lod);
  }
  else
  {
    printf("No pass: %d\n", pass);
  }
}

void vkGeometryNode::PrivScan(const vkClipper *clipper, IRenderer *renderer, IScanCallback *callback)
{
  callback->ScanGeometryNode(this);
}

