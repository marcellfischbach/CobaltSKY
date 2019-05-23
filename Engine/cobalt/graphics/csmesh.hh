#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/csenums.hh>
#include <csrefl/class.hh>
#include <cobalt/core/csresourcewrapper.hh>
#include <cobalt/graphics/imesh.hh>
#include <cobalt/math/csboundingbox.hh>
#include <map>
#include <vector>
#include <cobalt/graphics/csmesh.refl.hh>



namespace cs
{
struct iGraphics;
struct iIndexBuffer;
class Skeleton;
class SubMeshWrapper;
class Material;
struct iVertexBuffer;
struct iVertexDeclaration;



const csUInt32 InvalidMaterialIndex = ~0x00;

CS_CLASS()
class CSE_API Mesh : public CS_SUPER(cs::iMesh)
{
  CS_CLASS_GEN_OBJECT;
public:
  Mesh();
  virtual ~Mesh();

  void AddMesh(cs::SubMeshWrapper *instance, csSize materialIndex = 0, csUInt8 lod = 0, const std::string &name = "");
  csUInt8 GetNumberOfLODs() const;
  csSize GetNumberOfMeshes(csUInt8 lod = 0) const;
  cs::SubMeshWrapper *GetMesh(csUInt8 lod = 0, csSize idx = 0);
  const cs::SubMeshWrapper *GetMesh(csUInt8 lod = 0, csSize idx = 0) const;

  const cs::BoundingBox &GetBoundingBox() const;

  csSize GetMaterialIndex(csUInt8 lod = 0, csSize idx = 0) const;
  void OptimizeDataStruct();
  void UpdateBoundingBox();


  csSize GetNumberOfMaterials() const;
  void AddMaterialName(const std::string &materialName);
  const std::string &GetMaterialName(csSize idx) const;
  csUInt32 GetMaterialIndex(const std::string &materialName) const;

  virtual void Update(cs::iGraphics *renderer, const cs::Vector3f &cameraPos, csUInt64 frameNo);
  virtual void Render(cs::iGraphics *renderer, cs::eRenderPass pass, const std::vector<cs::Material *> &materials, csUInt8 lod = 0);

  csSize GetNumberOfRenderCalls(csUInt8 lod = 0) const;
  csSize GetNumberOfTotalTrigons(csUInt8 lod = 0) const;


protected:
  struct Data
  {
    cs::SubMeshWrapper *m_mesh;
    csSize m_materialIndex;
    std::string m_name;
  };

  struct LOD
  {
    std::vector<Data> m_meshes;
  };

  std::vector<LOD> m_lods;
  csSize m_numberOfMaterials;
  std::vector<std::string> m_materialNames;

  LOD &GetLOD(csUInt8 lod);

  cs::BoundingBox m_boundingBox;
};

CS_CLASS()
class CSE_API MeshWrapper : public CS_SUPER(cs::ResourceWrapper)
{
  CS_CLASS_GEN;
  CS_RESOURCE_WRAPPER(cs::Mesh, MeshWrapper, cs::ResourceWrapper);
};

}


CS_FORCEINLINE csSize cs::Mesh::GetNumberOfMaterials() const
{
  return m_numberOfMaterials;
}

CS_FORCEINLINE const cs::BoundingBox &cs::Mesh::GetBoundingBox() const
{
  return m_boundingBox;
}




