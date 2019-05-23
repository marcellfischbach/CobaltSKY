
#include <cobalt/entity/csgeometrymesh.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/csmaterialdef.hh>
#include <cobalt/graphics/csmultimaterial.hh>
#include <cobalt/graphics/csmesh.hh>



cs::GeometryMesh::GeometryMesh()
  : cs::GeometryData()
  , m_mesh(nullptr)
  , m_material(nullptr)
{
  m_localTransform.SetIdentity();
  m_globalTransform.SetIdentity();
}

cs::GeometryMesh::~GeometryMesh()
{

}


void cs::GeometryMesh::SetLocalTransorm(const cs::Matrix4f &localTransform)
{
  m_localTransform = localTransform;
}

const cs::Matrix4f &cs::GeometryMesh::GetLocalTransform() const
{
  return m_localTransform;
}

void cs::GeometryMesh::UpdateTransformation(const cs::Matrix4f &parentTransform)
{
  cs::Matrix4f::Mult(parentTransform, m_localTransform, m_globalTransform);
}


void cs::GeometryMesh::SetMesh(cs::MeshWrapper *mesh)
{
  CS_SET(m_mesh, mesh);
}

void cs::GeometryMesh::SetMaterial(cs::MultiMaterial *material)
{
  CS_SET(m_material, material);
}


void cs::GeometryMesh::Scan(cs::Clipper *clipper, cs::iGraphics *graphics, cs::iEntityScan *entityScan, const cs::ScanConfig &config)
{
  //entityScan->ScanGeometry(this);
}

void cs::GeometryMesh::Render(cs::iGraphics *graphics, cs::eRenderPass pass, csUInt8 lod)
{
  if (!m_material || !m_mesh)
  {
    return;
  }

  graphics->SetModelMatrix(m_globalTransform);
  //m_mesh->Render(graphics, pass, m_material, lod);
}



cs::GeometryData *cs::GeometryMesh::Clone()
{
  cs::GeometryMesh *mesh = new cs::GeometryMesh();
  mesh->SetAttached(false);
  mesh->SetLocalTransorm(m_localTransform);
  mesh->SetMesh(m_mesh);
  mesh->SetMaterial(m_material);
  mesh->SetName(GetName());
  return mesh;
}
