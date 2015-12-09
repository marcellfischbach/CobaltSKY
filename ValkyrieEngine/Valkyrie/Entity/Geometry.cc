
#include <Valkyrie/Entity/Geometry.hh>
#include <Valkyrie/Entity/Scan.hh>
#include <Valkyrie/Graphics/Mesh.hh>
#include <Valkyrie/Graphics/Material.hh>
#include <Valkyrie/Math/Clipper.hh>

vkGeometryBase::vkGeometryBase()
  : vkObject()
{

}

vkGeometryBase::~vkGeometryBase()
{

}










vkGeometryData::vkGeometryData()
  : vkGeometryBase()
  , m_mesh(0)
  , m_material(0)
{
  m_localTransform.SetIdentity();
  m_globalTransform.SetIdentity();
}

vkGeometryData::~vkGeometryData()
{

}


void vkGeometryData::SetLocalTransorm(const vkMatrix4f &localTransform)
{
  m_localTransform = localTransform;
}

const vkMatrix4f &vkGeometryData::GetLocalTransform() const
{
  return m_localTransform;
}

void vkGeometryData::UpdateTransform(const vkMatrix4f &parentTransform)
{
  vkMatrix4f::Mult(parentTransform, m_localTransform, m_globalTransform);
}


void vkGeometryData::SetMesh(vkMultiMesh *mesh)
{
  VK_SET(m_mesh, mesh);
}

void vkGeometryData::SetMaterial(vkMultiMaterial *material)
{
  VK_SET(m_material, material);
}


void vkGeometryData::Scan(vkClipper *clipper, IGraphics *graphics, IEntityScan *entityScan)
{
  entityScan->ScanGeometry(this);
}