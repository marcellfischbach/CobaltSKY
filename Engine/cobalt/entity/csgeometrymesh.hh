#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/core/csresourcewrapper.hh>
#include <cobalt/entity/csgeometrydata.hh>

#include <cobalt/entity/csgeometrymesh.refl.hh>

/**
* \ingroup entity
*/

namespace cs
{
class MeshWrapper;

CS_CLASS()
class CSE_API GeometryMesh : public CS_SUPER(cs::GeometryData)
{
  CS_CLASS_GEN;

public:
  GeometryMesh();
  virtual ~GeometryMesh();

  void SetMesh(cs::MeshWrapper * mesh);
  cs::MeshWrapper* GetMesh();
  const cs::MeshWrapper* GetMesh() const;

  void SetMaterial(cs::MultiMaterial * material);
  cs::MultiMaterial* GetMaterial();
  const cs::MultiMaterial* GetMaterial() const;

  void SetLocalTransorm(const cs::Matrix4f & localTransform);
  const cs::Matrix4f& GetLocalTransform() const;

  const cs::Matrix4f& GetGlobalTransform() const;

  void UpdateTransformation(const cs::Matrix4f & parentTransform);

  virtual void Scan(cs::Clipper * clipper, cs::iGraphics * graphics, cs::iEntityScan * entityScan, const cs::ScanConfig & config);

  virtual cs::GeometryData* Clone();

  void Render(cs::iGraphics * graphics, cs::eRenderPass pass, csUInt8 lod = 0);
private:
  cs::Matrix4f m_localTransform;
  cs::Matrix4f m_globalTransform;

  cs::MeshWrapper * m_mesh;
  cs::MultiMaterial * m_material;

};


CS_CLASS()
class CSE_API GeometryMeshWrapper : public CS_SUPER(cs::GeometryDataWrapper)
{
  CS_CLASS_GEN;
  CS_RESOURCE_WRAPPER(cs::GeometryMesh, GeometryMeshWrapper, cs::GeometryDataWrapper);
};

}




CS_FORCEINLINE cs::MeshWrapper *cs::GeometryMesh::GetMesh()
{
  return m_mesh;
}

CS_FORCEINLINE const cs::MeshWrapper *cs::GeometryMesh::GetMesh() const
{
  return m_mesh;
}


CS_FORCEINLINE cs::MultiMaterial *cs::GeometryMesh::GetMaterial()
{
  return m_material;
}

CS_FORCEINLINE const cs::MultiMaterial *cs::GeometryMesh::GetMaterial() const
{
  return m_material;
}


CS_FORCEINLINE const cs::Matrix4f &cs::GeometryMesh::GetGlobalTransform() const
{
  return m_globalTransform;
}


