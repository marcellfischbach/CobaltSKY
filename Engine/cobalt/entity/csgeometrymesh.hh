#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/core/csresourcewrapper.hh>
#include <cobalt/entity/csgeometrydata.hh>

#include <cobalt/entity/csgeometrymesh.refl.hh>

class csMeshWrapper;
/**
* \ingroup entity
*/

CS_CLASS()
class CSE_API csGeometryMesh : public CS_SUPER(csGeometryData)
{
  CS_CLASS_GEN;

public:
  csGeometryMesh();
  virtual ~csGeometryMesh();

  void SetMesh(csMeshWrapper *mesh);
  csMeshWrapper *GetMesh();
  const csMeshWrapper *GetMesh() const;

  void SetMaterial(csMultiMaterial *material);
  csMultiMaterial *GetMaterial();
  const csMultiMaterial *GetMaterial() const;

  void SetLocalTransorm(const csMatrix4f &localTransform);
  const csMatrix4f &GetLocalTransform() const;

  const csMatrix4f &GetGlobalTransform() const;

  void UpdateTransformation(const csMatrix4f &parentTransform);

  virtual void Scan(csClipper *clipper, iGraphics *graphics, iEntityScan *entityScan, const csScanConfig &config);

  virtual csGeometryData *Clone();

  void Render(iGraphics *graphics, csRenderPass pass, csUInt8 lod = 0);
private:
  csMatrix4f m_localTransform;
  csMatrix4f m_globalTransform;

  csMeshWrapper *m_mesh;
  csMultiMaterial *m_material;

};


CS_CLASS()
class CSE_API csGeometryMeshWrapper : public CS_SUPER(csGeometryDataWrapper)
{
  CS_CLASS_GEN;
  CS_RESOURCE_WRAPPER(csGeometryMesh, csGeometryMeshWrapper, csGeometryDataWrapper);
};






CS_FORCEINLINE csMeshWrapper *csGeometryMesh::GetMesh()
{
  return m_mesh;
}

CS_FORCEINLINE const csMeshWrapper *csGeometryMesh::GetMesh() const
{
  return m_mesh;
}


CS_FORCEINLINE csMultiMaterial *csGeometryMesh::GetMaterial()
{
  return m_material;
}

CS_FORCEINLINE const csMultiMaterial *csGeometryMesh::GetMaterial() const
{
  return m_material;
}


CS_FORCEINLINE const csMatrix4f &csGeometryMesh::GetGlobalTransform() const
{
  return m_globalTransform;
}


