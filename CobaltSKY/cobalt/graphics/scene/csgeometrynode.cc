

#include <cobalt/graphics/scene/csgeometrynode.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/csmaterial.hh>
#include <cobalt/graphics/csmesh.hh>


csGeometryNode::csGeometryNode(csMesh *mesh, csMaterial *material)
  : csSpatialNode()
  , m_mesh(0)
  , m_material(0)
{
  SetMesh(mesh);
  SetMaterial(material);
}

csGeometryNode::~csGeometryNode()
{

}


void csGeometryNode::SetMesh(csMesh* mesh)
{
  CS_SET(m_mesh, mesh);
}


void csGeometryNode::SetMaterial(csMaterial *material)
{
  CS_SET(m_material, material);
}


void csGeometryNode::Render(iGraphics *renderer, csRenderPass pass, csUInt8 lod)
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

void csGeometryNode::PrivScan(const csClipper *clipper, iGraphics *renderer, iScanCallback *callback)
{
  callback->ScanGeometryNode(this);
}

void csGeometryNode::UpdateBoundingBox(csBoundingBox &bbox)
{
  csSpatialNode::UpdateBoundingBox(bbox);

  if (m_mesh)
  {
//    bbox.Add(GetMatrix(), m_mesh->GetBoundingBox());
  }
  bbox.Finish();
}