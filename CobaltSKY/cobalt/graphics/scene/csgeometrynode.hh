#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/csenums.hh>
#include <cobalt/graphics/scene/csspatialnode.hh>
#include <cobalt/graphics/scene/csgeometrynode.refl.hh>

struct iGraphics;
class csMaterial;
class csMesh;

CS_CLASS()
class CSE_API csGeometryNode : public CS_SUPER(csSpatialNode)
{
  CS_CLASS_GEN;
public:
  csGeometryNode(csMesh *mesh = 0, csMaterial *material = 0);
  virtual ~csGeometryNode();


  void SetMesh(csMesh *mesh);
  csMesh *GetMesh();
  const csMesh *GetMesh() const;

  void SetMaterial(csMaterial *material);
  csMaterial *GetMaterial();
  const csMaterial *GetMaterial() const;

  void Render(iGraphics *renderer, csRenderPass renderPass, csUInt8 lod = 0);

protected:
  virtual void PrivScan(const csClipper *clipper, iGraphics *renderer, iScanCallback *callback);

  virtual void UpdateBoundingBox(csBoundingBox &bbox);

private:
  csMesh *m_mesh;
  csMaterial *m_material;

};


CS_FORCEINLINE csMesh *csGeometryNode::GetMesh()
{
  return m_mesh;
}

CS_FORCEINLINE const csMesh *csGeometryNode::GetMesh() const
{
  return m_mesh;
}


CS_FORCEINLINE csMaterial *csGeometryNode::GetMaterial()
{
  return m_material;
}

CS_FORCEINLINE const csMaterial *csGeometryNode::GetMaterial() const
{
  return m_material;
}

