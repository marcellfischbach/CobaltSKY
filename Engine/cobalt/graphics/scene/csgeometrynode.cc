

#include <cobalt/graphics/scene/csgeometrynode.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/csmaterial.hh>
#include <cobalt/graphics/csmesh.hh>


cs::GeometryNode::GeometryNode(cs::Mesh *mesh, cs::Material *material)
  : cs::SpatialNode()
  , m_mesh(0)
  , m_material(0)
{
  SetMesh(mesh);
  SetMaterial(material);
}

cs::GeometryNode::~GeometryNode()
{

}


void cs::GeometryNode::SetMesh(cs::Mesh* mesh)
{
  CS_SET(m_mesh, mesh);
}


void cs::GeometryNode::SetMaterial(cs::Material *material)
{
  CS_SET(m_material, material);
}


void cs::GeometryNode::Render(cs::iGraphics *renderer, cs::eRenderPass pass, csUInt8 lod)
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

void cs::GeometryNode::PrivScan(const cs::Clipper *clipper, cs::iGraphics *renderer, cs::iScanCallback *callback)
{
  callback->ScanGeometryNode(this);
}

void cs::GeometryNode::UpdateBoundingBox(cs::BoundingBox &bbox)
{
  cs::SpatialNode::UpdateBoundingBox(bbox);

  if (m_mesh)
  {
//    bbox.Add(GetMatrix(), m_mesh->GetBoundingBox());
  }
  bbox.Finish();
}