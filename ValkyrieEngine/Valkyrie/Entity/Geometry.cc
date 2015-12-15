
#include <Valkyrie/Entity/Geometry.hh>
#include <Valkyrie/Entity/Scan.hh>
#include <Valkyrie/Graphics/IGraphics.hh>
#include <Valkyrie/Graphics/Mesh.hh>
#include <Valkyrie/Graphics/Material.hh>
#include <Valkyrie/Math/Clipper.hh>

vkGeometryData::vkGeometryData()
  : vkObject()
  , m_attached(false)
{

}

vkGeometryData::~vkGeometryData()
{

}










vkGeometryMesh::vkGeometryMesh()
  : vkGeometryData()
  , m_mesh(0)
  , m_material(0)
{
  m_localTransform.SetIdentity();
  m_globalTransform.SetIdentity();
}

vkGeometryMesh::~vkGeometryMesh()
{

}


void vkGeometryMesh::SetLocalTransorm(const vkMatrix4f &localTransform)
{
  m_localTransform = localTransform;
}

const vkMatrix4f &vkGeometryMesh::GetLocalTransform() const
{
  return m_localTransform;
}

void vkGeometryMesh::UpdateTransformation(const vkMatrix4f &parentTransform)
{
  vkMatrix4f::Mult(parentTransform, m_localTransform, m_globalTransform);
}


void vkGeometryMesh::SetMesh(vkMesh *mesh)
{
  VK_SET(m_mesh, mesh);
}

void vkGeometryMesh::SetMaterial(vkMultiMaterial *material)
{
  VK_SET(m_material, material);
}


void vkGeometryMesh::Scan(vkClipper *clipper, IGraphics *graphics, IEntityScan *entityScan, const vkScanConfig &config)
{
  entityScan->ScanGeometry(this);
}

void vkGeometryMesh::Render(IGraphics *graphics, vkRenderPass pass, vkUInt8 lod)
{
  if (!m_material || !m_mesh)
  {
    return;
  }

  graphics->SetModelMatrix(m_globalTransform);
  m_mesh->Render(graphics, pass, m_material, lod);
}



vkGeometryData *vkGeometryMesh::Clone()
{
  vkGeometryMesh *mesh = new vkGeometryMesh();
  mesh->SetAttached(false);
  mesh->SetLocalTransorm(m_localTransform);
  mesh->SetMesh(m_mesh);
  mesh->SetMaterial(m_material);
  mesh->SetName(GetName());
  return mesh;
}