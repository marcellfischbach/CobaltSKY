#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/csenums.hh>
#include <cobalt/graphics/scene/csspatialnode.hh>
#include <cobalt/graphics/scene/csgeometrynode.refl.hh>


namespace cs
{
struct iGraphics;
class Material;
class Mesh;

CS_CLASS()
class CSE_API GeometryNode : public CS_SUPER(cs::SpatialNode)
{
  CS_CLASS_GEN;
public:
  GeometryNode(cs::Mesh * mesh = 0, cs::Material * material = 0);
  virtual ~GeometryNode();


  void SetMesh(cs::Mesh * mesh);
  cs::Mesh * GetMesh();
  const cs::Mesh * GetMesh() const;

  void SetMaterial(cs::Material * material);
  cs::Material * GetMaterial();
  const cs::Material * GetMaterial() const;

  void Render(cs::iGraphics * renderer, cs::eRenderPass renderPass, csUInt8 lod = 0);

protected:
  virtual void PrivScan(const cs::Clipper * clipper, cs::iGraphics * renderer, cs::iScanCallback * callback);

  virtual void UpdateBoundingBox(cs::BoundingBox & bbox);

private:
  cs::Mesh * m_mesh;
  cs::Material * m_material;

};

}


CS_FORCEINLINE cs::Mesh *cs::GeometryNode::GetMesh()
{
  return m_mesh;
}

CS_FORCEINLINE const cs::Mesh *cs::GeometryNode::GetMesh() const
{
  return m_mesh;
}


CS_FORCEINLINE cs::Material *cs::GeometryNode::GetMaterial()
{
  return m_material;
}

CS_FORCEINLINE const cs::Material *cs::GeometryNode::GetMaterial() const
{
  return m_material;
}

