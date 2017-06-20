#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/entity/csgeometrydata.hh>

#include <cobalt/entity/csgeometrymesh.refl.hh>

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

  void SetMesh(csMesh *mesh);
  csMesh *GetMesh();
  const csMesh *GetMesh() const;

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

  csMesh *m_mesh;
  csMultiMaterial *m_material;

};



CS_FORCEINLINE csMesh *csGeometryMesh::GetMesh()
{
  return m_mesh;
}

CS_FORCEINLINE const csMesh *csGeometryMesh::GetMesh() const
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


