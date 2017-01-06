

#include <valkyrie/graphics/scene/vkgeometrynode.hh>
#include <valkyrie/graphics/igraphics.hh>
#include <valkyrie/graphics/vkmaterialinstance.hh>
#include <valkyrie/graphics/vkmesh.hh>


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


void vkGeometryNode::Render(IGraphics *renderer, vkRenderPass pass, vkUInt8 lod)
{
  if (!m_material || !m_mesh)
  {
    return;
  }

  /*
  if (m_material->Bind(renderer, pass))
  {
    renderer->SetModelMatrix(GetMatrix());
    m_mesh->Render(renderer, );
  }
  else
  {
    printf("No pass: %d\n", pass);
  }
  */
}

void vkGeometryNode::PrivScan(const vkClipper *clipper, IGraphics *renderer, IScanCallback *callback)
{
  callback->ScanGeometryNode(this);
}

void vkGeometryNode::UpdateBoundingBox(vkBoundingBox &bbox)
{
  vkSpatialNode::UpdateBoundingBox(bbox);

  if (m_mesh)
  {
//    bbox.Add(GetMatrix(), m_mesh->GetBoundingBox());
  }
  bbox.Finish();
}