
#include <cobalt/entity/csgeometrymesh.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/csmaterialdef.hh>
#include <cobalt/graphics/csmultimaterial.hh>
#include <cobalt/graphics/csmesh.hh>



csGeometryMesh::csGeometryMesh()
  : csGeometryData()
  , m_mesh(0)
  , m_material(0)
{
  m_localTransform.SetIdentity();
  m_globalTransform.SetIdentity();
}

csGeometryMesh::~csGeometryMesh()
{

}


void csGeometryMesh::SetLocalTransorm(const csMatrix4f &localTransform)
{
  m_localTransform = localTransform;
}

const csMatrix4f &csGeometryMesh::GetLocalTransform() const
{
  return m_localTransform;
}

void csGeometryMesh::UpdateTransformation(const csMatrix4f &parentTransform)
{
  csMatrix4f::Mult(parentTransform, m_localTransform, m_globalTransform);
}


void csGeometryMesh::SetMesh(csMesh *mesh)
{
  CS_SET(m_mesh, mesh);
}

void csGeometryMesh::SetMaterial(csMultiMaterial *material)
{
  CS_SET(m_material, material);
}


void csGeometryMesh::Scan(csClipper *clipper, iGraphics *graphics, iEntityScan *entityScan, const csScanConfig &config)
{
  //entityScan->ScanGeometry(this);
}

void csGeometryMesh::Render(iGraphics *graphics, csRenderPass pass, csUInt8 lod)
{
  if (!m_material || !m_mesh)
  {
    return;
  }

  graphics->SetModelMatrix(m_globalTransform);
  //m_mesh->Render(graphics, pass, m_material, lod);
}



csGeometryData *csGeometryMesh::Clone()
{
  csGeometryMesh *mesh = new csGeometryMesh();
  mesh->SetAttached(false);
  mesh->SetLocalTransorm(m_localTransform);
  mesh->SetMesh(m_mesh);
  mesh->SetMaterial(m_material);
  mesh->SetName(GetName());
  return mesh;
}